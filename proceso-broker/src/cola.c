#include "cola.h"

void inicializar_colas_mensajeria(void (*procesar_pokemon)(t_cola_mensajeria* cola)){
	cola_mensajeria_new = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_new->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_new);
	cola_mensajeria_appeared = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_appeared->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_appeared);
	cola_mensajeria_get = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_get->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_get);
	cola_mensajeria_localized = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_localized->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_localized);
	cola_mensajeria_catch = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_catch->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_catch);
	cola_mensajeria_caught = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_caught->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_caught);
}

t_cola_mensajeria* cola_mensajeria_create(){
	t_cola_mensajeria* cola_mensajeria = malloc(sizeof(t_cola_mensajeria));
	cola_mensajeria->queue = queue_create();
	cola_mensajeria->suscriptores = list_create();
	sem_init(&cola_mensajeria->semaforoMensajes,0,0);
	sem_init(&cola_mensajeria->semaforoSuscriptores,0,0);
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
	//mensaje_mostrar(mensaje);
	queue_push(cola->queue, mensaje);
	sem_post(&cola->semaforoMensajes);
}

void inicializar_ids_mensajes(){
	ultimo_id = 0;
}
