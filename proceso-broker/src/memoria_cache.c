#include "memoria_cache.h"

t_memoria_cache* memoria_cache_create(){
	t_memoria_cache* memoria = malloc(sizeof(t_memoria_cache));
	memoria->memoria_cache = dictionary_create();
	return memoria;
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

	confirmar_suscripcion(parametros->suscriptor->socket);

	if(mensajes != NULL){
		int i;
		for (i = 0; i < list_size(mensajes); i++) {
			t_mensaje* mensaje = (t_mensaje*) list_get(mensajes, i);
			int resultado_envio = enviar_mensaje(mensaje,parametros->suscriptor->socket);

			char* mensaje_a_loggear = string_from_format("Enviado a %d a traves del socket %d ", parametros->suscriptor->pid,parametros->suscriptor->socket);
			string_append(&mensaje_a_loggear, mensaje_to_string(mensaje));
			log_info(loger2, mensaje_a_loggear);

			if (resultado_envio > 0) {
				recibir_ACK(parametros->suscriptor->socket);
			} else {
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

void obtener_logger2(t_log* logger){
	loger2 = logger;
}
