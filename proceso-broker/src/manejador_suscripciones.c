#include "manejador_suscripciones.h"

t_suscripciones* suscripciones_create(t_mensaje* mensaje, int* socket){
	t_suscripciones* info_suscripcion = malloc(sizeof(t_suscripciones));
	info_suscripcion->mensaje = mensaje;
	info_suscripcion->socket = socket;
	return info_suscripcion;
}

void suscripciones_destroy(t_suscripciones* suscripciones){
	mensaje_destroy(suscripciones->mensaje);
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
	//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
	confirmar_suscripcion(*socket);

	pthread_mutex_lock(&cola_suscripciones->mutex_cola_suscripcion);
	queue_push(cola_suscripciones->queue,(void*)info_suscripcion);
	pthread_mutex_unlock(&cola_suscripciones->mutex_cola_suscripcion);
	sem_post(&cola_suscripciones->semaforoSuscripcion);

}

void iniciar_procesador_suscripciones(void(*procesar_suscripcion)(void)){
	cola_suscripciones_create();
	pthread_create(&procesador_suscripciones,NULL,(void*)procesar_suscripcion,NULL);
	pthread_detach(procesador_suscripciones);
}

void memoria_cache_enviar_mensajes_cacheados(t_suscriptor* suscriptor, op_code cola){
	t_list* mensajes;

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
			//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
			int resultado_envio = enviar_mensaje(mensaje,suscriptor->socket);

			if (resultado_envio > 0) {
				loggear_envio_mensaje(mensaje_to_string(mensaje));
				//TODO verificar retorno de ack
				//TODO Syscall param socketcall.recv(args) points to uninitialised byte(s)
				recibir_ACK(suscriptor->socket);
				loggear_recepcion_ACK(suscriptor_to_string(suscriptor));
			} else {
				log_personal_error_envio_a_suscriptor(suscriptor_to_string(suscriptor));
				i = list_size(mensajes);
			}
		}
		list_destroy_and_destroy_elements(mensajes,(void*)mensaje_destroy);
	}else{
		//TODO pasar a logger personal
		puts("NO HAY MENSAJES DE ESA COLA");
	}
}
