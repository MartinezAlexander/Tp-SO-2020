#include "manejador_suscripciones.h"

t_suscripciones* suscripciones_create(t_mensaje* mensaje, int* socket){
	t_suscripciones* info_suscripcion = malloc(sizeof(t_suscripciones));
	info_suscripcion->mensaje = mensaje;
	info_suscripcion->socket = socket;
	return info_suscripcion;
}

void suscripciones_destroy(t_suscripciones* suscripciones){
	mensaje_destroy(suscripciones->mensaje);
	free(suscripciones->socket);
	free(suscripciones);
}

void cola_suscripciones_create(){
	cola_suscripciones = malloc(sizeof(t_cola_suscripciones));
	cola_suscripciones->queue = queue_create();
	sem_init(&cola_suscripciones->semaforoSuscripcion,0,0);
	pthread_mutex_init(&cola_suscripciones->mutex_cola_suscripcion,NULL);
}

void cola_suscripciones_agregar_suscripcion(t_mensaje* mensaje, int* socket){
	t_suscripciones* info_suscripcion = suscripciones_create(mensaje,socket);

	loggear_recepcion_mensaje(mensaje_to_string(mensaje));
	confirmar_suscripcion(*socket);

	pthread_mutex_lock(&cola_suscripciones->mutex_cola_suscripcion);
	queue_push(cola_suscripciones->queue,(void*)info_suscripcion);
	pthread_mutex_unlock(&cola_suscripciones->mutex_cola_suscripcion);
	sem_post(&cola_suscripciones->semaforoSuscripcion);

}

void iniciar_procesador_suscripciones(void(*procesar_suscripcion)(void)){
	cola_suscripciones_create();
	pthread_create(&procesador_suscripciones,NULL,(void*)procesar_suscripcion,NULL);
	//pthread_detach(procesador_suscripciones);
}

void memoria_cache_enviar_mensajes_cacheados(int socket, t_list* mensajes_enviados,int32_t pid , op_code cola){
	t_list* mensajes;
	t_suscriptor suscriptor;
	suscriptor.socket = socket;
	suscriptor.pid = pid;

	if(string_equals_ignore_case(algoritmo_memoria,"BS")){
		pthread_mutex_lock(&mutex_bs);
		mensajes = obtener_mensajes_cacheados_por_cola_bs(cola);
		pthread_mutex_unlock(&mutex_bs);
	}else{
		pthread_mutex_lock(&mutex_cacheo);
		mensajes = obtener_mensajes_cacheados_por_cola_pd(cola);
		pthread_mutex_unlock(&mutex_cacheo);
	}


	if(mensajes != NULL){
		int i;
		for (i = 0; i < list_size(mensajes); i++) {

			t_mensaje* mensaje = (t_mensaje*) list_get(mensajes, i);

			if(!suscriptor_ya_recibio_mensaje(mensajes_enviados,mensaje->id)){
				int resultado_envio = enviar_mensaje(mensaje,socket);

				if (resultado_envio > 0) {
					loggear_envio_mensaje(mensaje_to_string(mensaje));
					if(recibir_ACK(socket)){
						char* suscriptor_string = suscriptor_to_string(&suscriptor);
						loggear_recepcion_ACK(suscriptor_string);
						suscriptor_agregar_mensaje_recibido(mensajes_enviados,mensaje->id);
					}else{
						//TODO loguear que no se recibio el ack en el loguer personal
						i = list_size(mensajes);
					}
				} else {
					log_personal_error_envio_a_suscriptor(suscriptor_to_string(&suscriptor));
					i = list_size(mensajes);
				}
			}else{
				//TODO loguear que el mensaje ya se lo habia mandado al suscriptor
			}
		}
		list_destroy_and_destroy_elements(mensajes,(void*)mensaje_destroy);
	}else{
		//TODO pasar a logger personal
		puts("NO HAY MENSAJES DE ESA COLA");
	}
}

void procesador_suscripciones_liberar(){
	queue_destroy_and_destroy_elements(cola_suscripciones->queue,(void*)suscripciones_destroy);
	free(cola_suscripciones);
}
