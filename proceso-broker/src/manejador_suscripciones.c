#include "manejador_suscripciones.h"

para_envio_mensaje_cacheados* parametros_create(t_suscriptor* suscriptor, op_code cola, t_memoria_cache* memoria){
	para_envio_mensaje_cacheados* parametros = malloc(sizeof(para_envio_mensaje_cacheados));
	parametros->suscriptor = suscriptor;
	parametros->cola = cola;
	parametros->memoria = memoria;
	return parametros;
}

void memoria_cache_enviar_mensajes_cacheados(para_envio_mensaje_cacheados* parametros){
	t_list* mensajes;

	if(string_equals_ignore_case(algoritmo_memoria,"BS")){
		pthread_mutex_lock(&mutex_bs);
		mensajes = obtener_mensajes_cacheados_por_cola_bs(parametros->cola);
		pthread_mutex_unlock(&mutex_bs);
	}else{
		pthread_mutex_lock(&mutex_cacheo);
		mensajes = obtener_mensajes_cacheados_por_cola_pd(parametros->cola);
		pthread_mutex_unlock(&mutex_cacheo);
	}


	if(mensajes != NULL){
		int i;
		for (i = 0; i < list_size(mensajes); i++) {
			t_mensaje* mensaje = (t_mensaje*) list_get(mensajes, i);
			//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
			int resultado_envio = enviar_mensaje(mensaje,parametros->suscriptor->socket);

			if (resultado_envio > 0) {
				loggear_envio_mensaje(mensaje_to_string(mensaje));
				//TODO verificar retorno de ack
				//TODO Syscall param socketcall.recv(args) points to uninitialised byte(s)
				recibir_ACK(parametros->suscriptor->socket);
				loggear_recepcion_ACK(suscriptor_to_string(parametros->suscriptor));
			} else {
				log_personal_error_envio_a_suscriptor(suscriptor_to_string(parametros->suscriptor));
				i = list_size(mensajes);
			}
		}
		list_destroy_and_destroy_elements(mensajes,(void*)mensaje_destroy);
	}

	parametros_destroy(parametros);
}

void parametros_destroy(para_envio_mensaje_cacheados* parametros){
	free(parametros);
}
