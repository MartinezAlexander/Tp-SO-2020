#include "memoria_cache.h"

void memoria_cache_create(){
	memoria_cache = malloc(sizeof(t_memoria_cache));
	memoria_cache->memoria_cache = dictionary_create();
}

//TODO crear un semaforo mutex
void memoria_cache_agregar_mensaje(t_memoria_cache* memoria, t_mensaje* mensaje){
	char* cola = op_code_to_string(mensaje->codigo);
	int existen_mensajes = dictionary_has_key(memoria->memoria_cache,cola);
	if(existen_mensajes){
		t_list* mensajes = (t_list*)dictionary_get(memoria->memoria_cache,cola);
		list_add(mensajes,(void*)mensaje);
	}else{
		t_list* mensajes = list_create();
		list_add(mensajes,(void*)mensaje);
		dictionary_put(memoria->memoria_cache,cola,mensajes);
	}
}

//TODO crear un semaforo mutex
t_list* memoria_cache_obtener_mensajes_por_cola(t_memoria_cache* memoria, op_code cola_suscripcion){
	char* cola = op_code_to_string(cola_suscripcion);

	int existen_mensajes = dictionary_has_key(memoria->memoria_cache,cola);

	t_list* mensajes = NULL;

	if(existen_mensajes){
		mensajes = (t_list*)dictionary_get(memoria->memoria_cache,cola);
	}

	return mensajes;
}

//TODO crear un semaforo mutex
void memoria_cache_enviar_mensajes_cacheados(para_envio_mensaje_cacheados* parametros){
	t_list* mensajes = memoria_cache_obtener_mensajes_por_cola(parametros->memoria,parametros->cola);

	if(mensajes != NULL){
		int i;
		for (i = 0; i < list_size(mensajes); i++) {
			t_mensaje* mensaje = (t_mensaje*) list_get(mensajes, i);
			int resultado_envio = enviar_mensaje(mensaje,parametros->suscriptor->socket);

			if (resultado_envio > 0) {
				//TODO pasar a log personal
				loggear_envio_mensaje(mensaje_to_string(mensaje));

				//TODO verificar retorno de ack
				recibir_ACK(parametros->suscriptor->socket);
				loggear_recepcion_ACK(suscriptor_to_string(parametros->suscriptor));
			} else {
				log_personal_error_envio_a_suscriptor(suscriptor_to_string(parametros->suscriptor));
				i = list_size(mensajes);
			}
		}
	}

	parametros_destroy(parametros);
}

para_envio_mensaje_cacheados* parametros_create(t_suscriptor* suscriptor, op_code cola, t_memoria_cache* memoria){
	para_envio_mensaje_cacheados* parametros = malloc(sizeof(para_envio_mensaje_cacheados));
	parametros->suscriptor = suscriptor;
	parametros->cola = cola;
	parametros->memoria = memoria;
	return parametros;
}

void parametros_destroy(para_envio_mensaje_cacheados* parametros){
	free(parametros);
}
