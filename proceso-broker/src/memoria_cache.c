#include "memoria_cache.h"


void memoria_cache_create(){
	memoria_cache = malloc(sizeof(t_memoria_cache));
	memoria_cache->cache = malloc(tamano_memoria);
}

void memoria_cache_agregar_mensaje(t_mensaje* mensaje, int donde_agregar){
	void* stream_mensaje = mensaje_to_stream(mensaje);
	memcpy(memoria_cache->cache + donde_agregar, stream_mensaje, mensaje_size(mensaje));
	free(stream_mensaje);
}

t_mensaje* memoria_cache_leer_mensaje(int desde_donde, int cuanto_leer){
	return mensaje_from_stream(memoria_cache->cache + desde_donde, cuanto_leer);
}

int memoria_cache_es_un_mensaje_tipo(int desde_donde, int cuanto_leer, op_code cola){
	void* stream = malloc(cuanto_leer);
	memcpy(stream, memoria_cache->cache + desde_donde, cuanto_leer);
	op_code codigo = mensaje_stream_obtener_codigo(stream);
	int es_del_tipo = 0;
	if(codigo == cola){
		es_del_tipo = 1;
	}
	free(stream);
	return es_del_tipo;
}

void* memoria_cache_leer_stream(int desde_donde, int cuanto_leer){
	void* stream = malloc(cuanto_leer);
	memcpy(stream, memoria_cache->cache + desde_donde, cuanto_leer);
	return stream;
}

void memoria_cache_agregar_stream(void* stream, int donde_agregar, int cuanto_agregar){
	memcpy(memoria_cache->cache + donde_agregar, stream, cuanto_agregar);
	free(stream);
}

int memoria_cache_tamanio(){
	return tamano_memoria;
}

void memoria_cache_destroy(){
	free(memoria_cache->cache);
	free(memoria_cache);
}

