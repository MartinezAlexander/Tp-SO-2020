#include "planificacion.h"

void iniciar_hilos_entrenadores(){
	for(int i = 0 ; i < list_size(entrenadores) ; i++){
		t_entrenador* entrenador = list_get(entrenadores, i);
		sem_init(&(entrenador->semaforo), 0, 0);
		pthread_create(&(entrenador->hilo), NULL, ejecutar_hilo, entrenador);
	}
}

void esperar_hilos_entrenadores(){
	for(int i = 0 ; i < list_size(entrenadores) ; i++){
		t_entrenador* entrenador = list_get(entrenadores, i);
		pthread_join(entrenador->hilo, NULL);
	}
}

void entrenador_entrar_a_planificacion(t_pokemon* pokemon){

	//Filtro entrenadores disponibles
	t_list* entrenadores_disponibles = list_filter(entrenadores, (void*)entrenador_disponible);

	//Obtengo entrenador mas cercano al pokemon a atrapar
	t_entrenador *entrenador_mas_cercano = obtener_entrenador_mas_cercano(entrenadores_disponibles, pokemon->posicion);

	//Le doy el objetivo actual al entrenador
	entrenador_mas_cercano->objetivo_actual = pokemon;

	//Agrego entrenador a la cola del planificador
	encolar(entrenador_mas_cercano);

	//En caso de que no haya nadie ejecutando en este instante, nadie me va a poder mandar
	// a ejecutar a mi que estoy en la cola, por eso es que tengo que comprobar esto.
	//Entonces si no hay nadie en ejecucion, planifico al proximo en la cola
	if(!hay_alguien_en_ejecucion() || planificador->algoritmo_planificacion == SJF_CD){
    	planificar();
    }
}

//TODO solucionar el tema del catch asincronico
//Entonces. Hay que tener otro hilo para el planificador
//este hilo solo ejecuta la funcion planificar();
//y esta sincronizado en orden con el entrenador
//de manera que no ejecuta hasta que el entrenador
//no ejecuta antes.
//Entonces el entrenador a fin de ciclo manda un post
//para el planificador.
//En el catch, entonces podemos mandar ese post antes de
//hacer el envio y recibimiento, y hacerlo asincronicamente
//porque libero el planificador para que mande al proximo
//Ojo: cuando salgo de ejecutar el entrenador tengo que
//chequear si ya mande el post antes de mandarlo de nuevo.

void ejecutar_hilo(t_entrenador* entrenador){
	//El entrenador debe ejecutar constantemente
	//y tomamos un ciclo como una pasada del while.
	//Decimos que el entrenador ya termino de ejcutar
	//cuando cumplio sus objetivos (y el del team)
	//esto lo verificamos a traves de su estado
	while(entrenador->estado != EXIT){
		sem_wait(&(entrenador->semaforo));

		ejecutar_entrenador(entrenador);

		planificar();
	}
}

void planificar(){
	//Para evitar problemas de sincronizacion agregamos un mutex
	sem_wait(&planificador->semaforo);
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
	sem_post(&planificador->semaforo);
}

int entrenador_disponible(t_entrenador *entrenador)
{
	return entrenador->estado == NEW || entrenador->estado == BLOCKED;
}
