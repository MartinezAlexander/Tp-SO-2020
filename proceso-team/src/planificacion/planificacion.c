#include "planificacion.h"


void planificador_iniciar_hilo_entrenador(t_entrenador* entrenador){
	sem_init(&(entrenador->semaforo), 0, 0);

	void* planificacion_correspondiente;

	//Elijo el planificador correspondiente
	switch(planificador->algoritmo_planificacion){
		case FIFO:
			planificacion_correspondiente = ejecutar_hilo_fifo;
			break;
		case RR:
			planificacion_correspondiente = ejecutar_hilo_rr;
			break;
		case SJF_SD:
			planificacion_correspondiente = ejecutar_hilo_sjf_sin_desalojo;
			break;
		case SJF_CD:
			planificacion_correspondiente = ejecutar_hilo_sjf_con_desalojo;
			break;
	}

	pthread_create(&(entrenador->hilo), NULL, planificacion_correspondiente, entrenador);
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

	//Planificador comprueba si hay alguien ejecutando
		//En caso contrario pone a ejecutar al primero de la cola
	if(puedo_ejecutar()){
		ejecutar_proximo();
	}
}

void ejecutar_hilo_fifo(t_entrenador* entrenador){
	while(1){
		comenzar_ciclo_fifo(entrenador);

		ejecutar_entrenador(entrenador);

		finalizar_ciclo_fifo(entrenador);
	}
}

void ejecutar_hilo_rr(t_entrenador* entrenador){
	while(1){
		comenzar_ciclo_rr(entrenador);

		ejecutar_entrenador(entrenador);

		finalizar_ciclo_rr(entrenador);
	}
}

void ejecutar_hilo_sjf_sin_desalojo(t_entrenador* entrenador){
	while(1){
		comenzar_ciclo_sjf_sin_desalojo(entrenador);

		ejecutar_entrenador(entrenador);

		finalizar_ciclo_sjf_sin_desalojo(entrenador);
	}
}

void ejecutar_hilo_sjf_con_desalojo(t_entrenador* entrenador){
	while(1){
		comenzar_ciclo_sjf_con_desalojo(entrenador);

		ejecutar_entrenador(entrenador);

		finalizar_ciclo_sjf_con_desalojo(entrenador);
	}
}

void ejecutar_proximo(){
	switch(planificador->algoritmo_planificacion){
		case FIFO:
			ejecutar_proximo_fifo();
			break;
		case RR:
			ejecutar_proximo_rr();
			break;
		case SJF_SD:
			ejecutar_proximo_sjf_sin_desalojo();
			break;
		case SJF_CD:
			ejecutar_proximo_sjf_con_desalojo();
			break;
	}
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

int puedo_ejecutar()
{
    return !list_any_satisfy(entrenadores, (void*)entrenador_en_ejecucion);
}

