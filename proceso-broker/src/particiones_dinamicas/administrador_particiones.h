#ifndef ADMINISTRADOR_PARTICIONES_H_
#define ADMINISTRADOR_PARTICIONES_H_

#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <mensajes/mensajes.h>
#include "particiones_dinamicas.h"
#include "../memoria_cache.h"
#include "../suscriptor.h"
#include "../logger.h"

typedef struct{
	t_suscriptor* suscriptor;
	op_code cola;
	t_memoria_cache* memoria;
}para_envio_mensaje_cacheados;

t_list* particiones;

int busquedas_fallidas;

void iniciar_administrador();

int first_fit(t_mensaje* mensaje);

void best_fit();

int es_hora_de_compactar();

void procedimiento_para_almacenamiento_de_datos(t_mensaje* mensaje, int(*algorimo)(t_mensaje* mensaje));

void administrador_cachear_mensaje(t_mensaje* mensaje);

t_list* obtener_mensajes_cacheados_por_cola(op_code cola);

void memoria_cache_enviar_mensajes_cacheados(para_envio_mensaje_cacheados* parametros);

para_envio_mensaje_cacheados* parametros_create(t_suscriptor* suscriptor, op_code cola, t_memoria_cache* memoria);

void parametros_destroy(para_envio_mensaje_cacheados* parametros);

#endif
