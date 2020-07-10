#include "cola.h"

void inicializar_colas_mensajeria(void (*procesar_pokemon)(t_cola_mensajeria* cola)){
	cola_mensajeria_new = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_new->hilo), NULL, (void*) procesar_pokemon,cola_mensajeria_new);
	//pthread_detach(cola_mensajeria_new->hilo);

	cola_mensajeria_appeared = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_appeared->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_appeared);
	//pthread_detach(cola_mensajeria_appeared->hilo);

	cola_mensajeria_get = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_get->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_get);
	//pthread_detach(cola_mensajeria_get->hilo);

	cola_mensajeria_localized = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_localized->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_localized);
	//pthread_detach(cola_mensajeria_localized->hilo);

	cola_mensajeria_catch = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_catch->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_catch);
	//pthread_detach(cola_mensajeria_catch->hilo);

	cola_mensajeria_caught = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_caught->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_caught);
	//pthread_detach(cola_mensajeria_caught->hilo);

}

t_cola_mensajeria* cola_mensajeria_create(){
	t_cola_mensajeria* cola_mensajeria = malloc(sizeof(t_cola_mensajeria));
	cola_mensajeria->queue = queue_create();
	cola_mensajeria->suscriptores = list_create();
	sem_init(&cola_mensajeria->semaforoMensajes,0,0);
	//sem_init(&cola_mensajeria->semaforoSuscriptores,0,0);
	//TODO Address 0x4294704 is 28 bytes inside a block of size 76 alloc'd
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

void cola_mensajeria_recibir_mensaje(t_cola_mensajeria* cola, t_mensaje* mensaje, int* ultimo_id, int* socket){
	pthread_mutex_lock(&mutex_id);
	(*ultimo_id)++;
	mensaje->id = (*ultimo_id);
	enviar_id(*socket, *ultimo_id);
	pthread_mutex_unlock(&mutex_id);

	loggear_recepcion_mensaje(mensaje_to_string(mensaje));
	pthread_mutex_lock(&cola->mutex_cola_mensaje);
	queue_push(cola->queue, mensaje);
	pthread_mutex_unlock(&cola->mutex_cola_mensaje);
	sem_post(&cola->semaforoMensajes);

	free(socket);
}

void inicializar_ids_mensajes(){
	ultimo_id = 0;
	pthread_mutex_init(&mutex_id, NULL);
}

void colas_mensajeria_liberar(){

	list_destroy_and_destroy_elements(cola_mensajeria_new->suscriptores,(void*)suscriptor_destroy);
	list_destroy_and_destroy_elements(cola_mensajeria_appeared->suscriptores,(void*)suscriptor_destroy);
	list_destroy_and_destroy_elements(cola_mensajeria_caught->suscriptores,(void*)suscriptor_destroy);
	list_destroy_and_destroy_elements(cola_mensajeria_catch->suscriptores,(void*)suscriptor_destroy);
	list_destroy_and_destroy_elements(cola_mensajeria_get->suscriptores,(void*)suscriptor_destroy);
	list_destroy_and_destroy_elements(cola_mensajeria_localized->suscriptores,(void*)suscriptor_destroy);

	queue_destroy_and_destroy_elements(cola_mensajeria_new->queue,(void*)mensaje_destroy);
	queue_destroy_and_destroy_elements(cola_mensajeria_appeared->queue,(void*)mensaje_destroy);
	queue_destroy_and_destroy_elements(cola_mensajeria_caught->queue,(void*)mensaje_destroy);
	queue_destroy_and_destroy_elements(cola_mensajeria_catch->queue,(void*)mensaje_destroy);
	queue_destroy_and_destroy_elements(cola_mensajeria_get->queue,(void*)mensaje_destroy);
	queue_destroy_and_destroy_elements(cola_mensajeria_localized->queue,(void*)mensaje_destroy);

	free(cola_mensajeria_new);
	free(cola_mensajeria_appeared);
	free(cola_mensajeria_caught);
	free(cola_mensajeria_catch);
	free(cola_mensajeria_get);
	free(cola_mensajeria_localized);
}
