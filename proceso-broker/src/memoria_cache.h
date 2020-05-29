#ifndef MEMORIA_CACHE_H_
#define MEMORIA_CACHE_H_

//TODO implementacion de memoria cache como un diccionario hasta ver la teoria de memoria
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <mensajes/mensajes.h>
#include "suscriptor.h"
#include <stdlib.h>

typedef struct{
	t_dictionary* memoria_cache;
}t_memoria_cache;

typedef struct{
	t_suscriptor* suscriptor;
	op_code cola;
	t_memoria_cache* memoria;
}para_envio_mensaje_cacheados;

t_memoria_cache* memoria_cache_create();

void memoria_cache_agregar_mensaje(t_memoria_cache* memoria,t_mensaje* mensaje);

t_list* memoria_cache_obtener_mensajes_por_cola(t_memoria_cache* memoria,op_code cola);

void memoria_cache_enviar_mensajes_cacheados(para_envio_mensaje_cacheados* parametros);

para_envio_mensaje_cacheados* parametros_create(t_suscriptor* suscriptor, op_code cola, t_memoria_cache* memoria);

void parametros_destroy(para_envio_mensaje_cacheados* parametros);

#endif
