#include "planificacion.h"

void iniciar_hilos_planificacion(){
	pthread_create(&(planificador->hilo_planificacion), NULL, (void*)ejecutar_hilo_planificador, NULL);
	for(int i = 0 ; i < list_size(entrenadores) ; i++){
		t_entrenador* entrenador = list_get(entrenadores, i);
		sem_init(&(entrenador->semaforo), 0, 0);
		pthread_create(&(entrenador->hilo), NULL, (void*)ejecutar_hilo, entrenador);
	}
}

void esperar_hilos_planificacion(){
	for(int i = 0 ; i < list_size(entrenadores) ; i++){
		t_entrenador* entrenador = list_get(entrenadores, i);
		pthread_join(entrenador->hilo, NULL);
	}
	pthread_join(planificador->hilo_planificacion, NULL);
}



void entrenador_entrar_a_planificacion(t_pokemon* pokemon){

	//Filtro entrenadores disponibles
	t_list* entrenadores_disponibles = list_filter(entrenadores, (void*)entrenador_disponible);

	//Puede pasar que todos mis entrenadores esten ocupados, en ese caso
	//voy a guardar el pokemon en la cola de espera
	if(list_is_empty(entrenadores_disponibles)){

		pthread_mutex_lock(&mutex_cola_espera);
		queue_push(cola_pokemones_en_espera, pokemon);
		pthread_mutex_unlock(&mutex_cola_espera);

		printf("[Pokemon] Pokemon puesto en espera, motivo: sin entrenadores disponibles\n");
		return;
		//Ademas no voy a seguir con el procedimiento normal
	}

	//Obtengo entrenador mas cercano al pokemon a atrapar
	t_entrenador *entrenador_mas_cercano = obtener_entrenador_mas_cercano(entrenadores_disponibles, pokemon->posicion);
	list_destroy(entrenadores_disponibles);

	//Le doy el objetivo actual al entrenador
	entrenador_mas_cercano->objetivo_actual = pokemon;

	int debo_planificar = !hay_alguien_en_ejecucion();

	//En caso de que este usando planificacion SJF con desalojo
	//antes de encolar el nuevo, debo desalojar el entrenador actual
	//y encolarlo de nuevo
	if(planificador->algoritmo_planificacion == SJF_CD){

		//La idea es que yo debo desalojar al actual y volver a encolarlo,
		//pero para eso tengo que esperar a que termine la ejecucion
		//del ciclo actual. Por eso es que tengo un mutex compartido
		//para esta seccion con la del hilo de ejecucion, y lo unico que
		//hago aca es marcar que se tiene que desalojar, y al final del
		//ciclo voy a chequear si debo ser desalojado y hacerlo si corresponde

		pthread_mutex_lock(&(planificador->mutex_desalojo));

		if(hay_alguien_en_ejecucion()){
			planificador->debo_desalojar_al_fin_de_ciclo = 1;

			pthread_mutex_unlock(&(planificador->mutex_desalojo));
			//Aviso que no voy a planificar ya que lo hace el entrenador
			debo_planificar = 0;
			//Luego de marcar el desalojo, espero a que me habilite el entrenador de nuevo
			sem_wait(&planificador->semaforo_desalojo);
			//Encolo
			encolar(entrenador_mas_cercano);
			//Signal
			sem_post(&planificador->semaforo_post_desalojo);
			//debo desalojar pasa a 0
			planificador->debo_desalojar_al_fin_de_ciclo = 0;
		}else{
			pthread_mutex_unlock(&(planificador->mutex_desalojo));
			encolar(entrenador_mas_cercano);
		}


		//Yo aca solo tengo que sacarlo de ejecucion.
		//Lo que logro con esto es que el entrenador actual va a terminar de ejecutar
		//su ciclo, y luego como va a ser el turno del planificador, se va a fijar
		//si hay alguien ejecutando y va a re-planificar

		//Por esto mismo es que si yo desalojo a alguien, debo evitar la
		//llamada a planificar() que se hace mas abajo, ya que voy a replanificar
		//a traves del hilo de ejecucion del entrenador.
		//Sino, estaria planificando dos veces

	}else{
		encolar(entrenador_mas_cercano);
	}

	//En caso de que no haya nadie ejecutando en este instante, nadie me va a poder mandar
	// a ejecutar a mi que estoy en la cola, por eso es que tengo que comprobar esto.
	//Entonces si no hay nadie en ejecucion, planifico al proximo en la cola
	if(debo_planificar){
    	planificar();
    }
}

//En un principio pensabamos en tener tantos hilos como entrenadores
//para la ejecucion de los mismos, empezando todos bloqueados
//y desbloqueandose a medida que debemos ejecutarlos (por decision del
//planificador). Cada entrenador ejecutaba un ciclo, y al final del mismo
//chequeaba si debia re-planificar.
//El problema de este modelo es que debido a la secuencialidad del codigo,
//cuando un entrenador llegaba al objetivo y debia hacer el CATCH,
//no podria volver a re-planificar (que en ese caso siempre seria
//mandar al proximo entrenador) hasta que se envie el mensaje al broker
//y le devuelva la respuesta, etc.

//Nosotros pensamos que el siguiente entrenador no deberia tener que esperar
//a que el otro haga todas estas cosas para poder entrar en ejecucion,
//por lo que buscamos una alternativa en la que ni bien llega a su
//objetivo, mientras manda el catch el siguiente ya empieza a ejecutar.

//Como dijimos antes, por la secuencialidad del codigo, la solucion
//a la que llegamos es la de tener un hilo aparte que se encargue de planificar
//sacandole esa responsabilidad al entrenador (esto ademas tiene sentido ya que
//el entrenador no tiene porque saber como se planifica).
//De todas formas decidimos mantener el mismo formato de ejecucion-chequeo planif.
//por lo que estos dos hilos se ejecutaran en orden: entrenador-planificador
//(lo cual logramos con dos semaforos). De esta manera, el entrenador puede
//habilitar el semaforo del planificador una vez que llego al objetivo, antes de
//mandar el catch, por lo que solucionariamos el problema que describimos
//ya que el hilo este podra decidir quien planificar y no perdemos tiempo
//innecesariamente.
void ejecutar_hilo_planificador(){
	while(!finalizo_team){
		sem_wait(&(semaforo_planificacion));

		planificar();
		//Aca tendria que habilitar el semaforo
		//del entrenador, pero eso se hace dentro
		//de planificar()
	}
}

void ejecutar_hilo(t_entrenador* entrenador){
	//El entrenador debe ejecutar constantemente
	//y tomamos un ciclo como una pasada del while.
	//Decimos que el entrenador ya termino de ejcutar
	//cuando cumplio sus objetivos (y el del team)
	//esto lo verificamos a traves de su estado
	while(entrenador->estado != EXIT && entrenador->estado != BLOCKED_DEADLOCK){
		sem_wait(&(entrenador->semaforo));

		//Puede pasar que el entrenador pase a DEADLOCK, pero lo haga cuando reciba
		//la respuesta del caught. En ese caso ya se habria bloqueado el entrenador,
		//por lo que no saldria del while hasta el proximo ciclo.
		//Decidimos entonces que cuando llega un caught y cambio el estado mando signal
		//al semaforo de arriba para salir si corresponde
		if(entrenador->estado == EXIT || entrenador->estado == BLOCKED_DEADLOCK){
			break;
		}

		int termino_ejecucion = ejecutar_entrenador(entrenador);

		//Aca me fijo si me deben desalojar
		//En ese caso, me encolo y me saco de exec
		//y le doy signal al semaforo para que encole al nuevo
		pthread_mutex_lock(&(planificador->mutex_desalojo));
		if(planificador->debo_desalojar_al_fin_de_ciclo){
			sacar_de_ejecucion();
			encolar(entrenador);
			sem_post(&planificador->semaforo_desalojo);
			//WAIT antes de seguir a que se encole el nuevo,
			//asi no entro a re-planificar antes
			sem_wait(&planificador->semaforo_post_desalojo);
		}
		pthread_mutex_unlock(&(planificador->mutex_desalojo));

		//Ahora tengo que habilitar el semaforo del
		//planificador para que vea que hacer,
		//pero solo lo voy a habilitar si sigo en EXEC,
		//ya que si cambie de estado a BLOCKED_BY_CATCH
		//asumo que ya mande este post
		if(!termino_ejecucion){
			sem_post(&semaforo_planificacion);
		}
	}

	//La idea es que primero ejecute normalmente, y cuando termine de ejecutar
	//(por EXIT o DEADLOCK) se fijara aca si tiene deadlock, y en ese caso entra
	//en este nuevo loop hsata que lo resuelva
	while(entrenador->estado != EXIT){
		sem_wait(&(entrenador->semaforo));

		ejecutar_entrenador_intercambio_deadlock(entrenador);

		sem_post(&semaforo_planificacion);
	}
}

void planificar(){
	//Para evitar problemas de sincronizacion agregamos un mutex
	pthread_mutex_lock(&(planificador->mutex_planificacion));
	//Dependiendo del tipo de algoritmo el planificador funcionara
	//de distintas maneras
	switch(planificador->algoritmo_planificacion){
		case FIFO:
			planificar_fifo();
			break;
		case RR:
			planificar_rr();
			break;
		case SJF_SD:
			planificar_sjf_sd();
			break;
		case SJF_CD:
			planificar_sjf_cd();
			break;
	}
	pthread_mutex_unlock(&(planificador->mutex_planificacion));
}

int entrenador_disponible(t_entrenador *entrenador)
{
	return entrenador->estado == NEW || entrenador->estado == BLOCKED;
}
