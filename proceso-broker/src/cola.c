#include "cola.h"

void inicializar_colas_mensajeria(void (*procesar_pokemon)(t_cola_mensajeria* cola)){
	cola_mensajeria_new = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_new->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_new);
	pthread_detach(cola_mensajeria_new->hilo);

	cola_mensajeria_appeared = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_appeared->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_appeared);
	pthread_detach(cola_mensajeria_appeared->hilo);

	cola_mensajeria_get = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_get->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_get);
	pthread_detach(cola_mensajeria_get->hilo);

	cola_mensajeria_localized = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_localized->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_localized);
	pthread_detach(cola_mensajeria_localized->hilo);

	cola_mensajeria_catch = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_catch->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_catch);
	pthread_detach(cola_mensajeria_catch->hilo);

	cola_mensajeria_caught = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_caught->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_caught);
	pthread_detach(cola_mensajeria_caught->hilo);
	//TODO 136 bytes in 1 block are possibly lost in loss record 56 of 67 (join)
}

t_cola_mensajeria* cola_mensajeria_create(){
	t_cola_mensajeria* cola_mensajeria = malloc(sizeof(t_cola_mensajeria));
	cola_mensajeria->queue = queue_create();
	cola_mensajeria->suscriptores = list_create();
	sem_init(&cola_mensajeria->semaforoMensajes,0,0);
	//sem_init(&cola_mensajeria->semaforoSuscriptores,0,0);
	pthread_mutex_init(&cola_mensajeria->semaforoSuscriptores,NULL);
	pthread_mutex_init(&cola_mensajeria->mutex_cola_mensaje,NULL);
	return cola_mensajeria;
}

t_cola_mensajeria* cola_mensajeria_obtener(op_code codigo){
	t_cola_mensajeria* cola;
	switch(codigo){
		case NEW_POKEMON:
			cola = cola_mensajeria_new;
			break;
		case LOCALIZED_POKEMON:
			cola = cola_mensajeria_localized;
			break;
		case GET_POKEMON:
			cola = cola_mensajeria_get;
			break;
		case APPEARED_POKEMON:
			cola = cola_mensajeria_appeared;
			break;
		case CATCH_POKEMON:
			cola = cola_mensajeria_catch;
			break;
		case CAUGHT_POKEMON:
			cola = cola_mensajeria_caught;
			break;
		default:
			break;
	}
	return cola;
}

void cola_mensajeria_recibir_mensaje(t_cola_mensajeria* cola, t_mensaje* mensaje, int* ultimo_id){
	(*ultimo_id)++;
	mensaje->id = (*ultimo_id);

	loggear_recepcion_mensaje(mensaje_to_string(mensaje));
	pthread_mutex_lock(&cola->mutex_cola_mensaje);
	queue_push(cola->queue, mensaje);
	pthread_mutex_unlock(&cola->mutex_cola_mensaje);
	sem_post(&cola->semaforoMensajes);
}

void inicializar_ids_mensajes(){
	ultimo_id = 0;
}
