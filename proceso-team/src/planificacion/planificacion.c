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

//TODO: funcion para chequear entrenadores disponoibles,
//si hay alguin pokemones en espera, y si hay, planificarlos
//Idea: lo importante es encontrar donde podemos llamar a esta funcion
//tiene que ser cuando un entrenador termina de ejecutar.
//Estabamos viendo opciones y la que aparecia mejor
//era llamarla en sacar_de_ejecucion() (funcion de planificador)
//ya que no tengo que tocar los planificadores y siempre parece llamarse cuando
//saco a uno porque termino de ejecutar

//El tema es que para ver que estemos en lo correcto tendriamos
//que terminar de hacer sjf con desalojo

//TODO: Agregar mutex compartido entre esta funcion y la de arriba
void entrenador_entrar_a_planificacion(t_pokemon* pokemon){

	//Filtro entrenadores disponibles
	t_list* entrenadores_disponibles = list_filter(entrenadores, (void*)entrenador_disponible);
	//TODO catchear error de que no haya disponibles - en ese caso:
	//Si no hay entrenadores tengo que guardarme los pokemones en
	//la cola de espera

	//Obtengo entrenador mas cercano al pokemon a atrapar
	t_entrenador *entrenador_mas_cercano = obtener_entrenador_mas_cercano(entrenadores_disponibles, pokemon->posicion);
	list_destroy(entrenadores_disponibles);

	//Le doy el objetivo actual al entrenador
	entrenador_mas_cercano->objetivo_actual = pokemon;

	//Agrego entrenador a la cola del planificador
	encolar(entrenador_mas_cercano);

	//En caso de que no haya nadie ejecutando en este instante, nadie me va a poder mandar
	// a ejecutar a mi que estoy en la cola, por eso es que tengo que comprobar esto.
	//Entonces si no hay nadie en ejecucion, planifico al proximo en la cola
	if(!hay_alguien_en_ejecucion()/* || planificador->algoritmo_planificacion == SJF_CD*/){
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
	while(entrenador->estado != EXIT){
		sem_wait(&(entrenador->semaforo));
		int termino_ejecucion = ejecutar_entrenador(entrenador);

		//Ahora tengo que habilitar el semaforo del
		//planificador para que vea que hacer,
		//pero solo lo voy a habilitar si sigo en EXEC,
		//ya que si cambie de estado a BLOCKED_BY_CATCH
		//asumo que ya mande este post
		if(!termino_ejecucion){
			sem_post(&semaforo_planificacion);
		}
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
