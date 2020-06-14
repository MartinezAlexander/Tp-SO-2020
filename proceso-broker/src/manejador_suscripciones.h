#ifndef MANEJADOR_SUSCRIPCIONES_H_
#define MANEJADOR_SUSCRIPCIONES_H_

#include <stdlib.h>
#include <mensajes/mensajes.h>
#include "suscriptor.h"
#include "memoria_cache.h"
#include "logger.h"
#include "config.h"
#include <commons/string.h>
#include "particiones_dinamicas/administrador_particiones.h"
#include "buddy_system/administrador_buddy_system.h"

typedef struct{
	t_suscriptor* suscriptor;
	op_code cola;
	t_memoria_cache* memoria;
}para_envio_mensaje_cacheados;

para_envio_mensaje_cacheados* parametros_create(t_suscriptor* suscriptor, op_code cola, t_memoria_cache* memoria);

void memoria_cache_enviar_mensajes_cacheados(para_envio_mensaje_cacheados* parametros);

void parametros_destroy(para_envio_mensaje_cacheados* parametros);

#endif
