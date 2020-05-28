#include "planificacion.h"


void planificador_iniciar_hilo_entrenador(t_entrenador* entrenador){
	sem_init(&(entrenador->semaforo), 0, 0);
	pthread_create(&(entrenador->hilo), NULL, ejecutar_hilo, entrenador);
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
	if(!hay_alguien_en_ejecucion()){
    	planificar();
    }
}

void ejecutar_hilo(t_entrenador* entrenador){
	while(1){
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

void encolar(t_entrenador* entrenador)
{
	printf("Encolado entrenador en [%d , %d]\n", entrenador->posicion.posicionX,  entrenador->posicion.posicionY);
    entrenador->estado = READY;
	queue_push(planificador->cola, entrenador);
}
