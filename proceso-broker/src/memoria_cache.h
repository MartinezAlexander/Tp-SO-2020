#ifndef MEMORIA_CACHE_H_
#define MEMORIA_CACHE_H_

#include <commons/collections/list.h>
#include <mensajes/mensajes.h>
#include <stdlib.h>
#include "config.h"
#include "logger.h"
#include <commons/string.h>

typedef struct{
	void* cache;
}t_memoria_cache;

t_memoria_cache* memoria_cache;

void memoria_cache_create();

void memoria_cache_agregar_mensaje(t_mensaje* mensaje, int donde_agregar);

t_mensaje* memoria_cache_leer_mensaje(int desde_donde, int cuanto_leer);

int memoria_cache_es_un_mensaje_tipo(int desde_donde, int cuanto_leer, op_code cola);

void* memoria_cache_leer_stream(int desde_donde, int cuanto_leer);

void memoria_cache_agregar_stream(void* stream, int donde_agregar, int cuanto_agregar);

int memoria_cache_tamanio();

void memoria_cache_destroy();

#endif
