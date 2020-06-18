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
#include <semaphore.h>

typedef struct{
	t_mensaje* mensaje;
	int* socket;
}t_suscripciones;

typedef struct{
	t_queue* queue;
	sem_t semaforoSuscripcion;
	pthread_mutex_t mutex_cola_suscripcion;
}t_cola_suscripciones;

t_cola_suscripciones* cola_suscripciones;
pthread_t procesador_suscripciones;

t_suscripciones* suscripciones_create(t_mensaje* mensaje, int* socket);
void suscripciones_destroy(t_suscripciones* suscripciones);
void memoria_cache_enviar_mensajes_cacheados(int socket, t_list* mensajes_enviados,int32_t pid , op_code cola);
void cola_suscripciones_create();
void iniciar_procesador_suscripciones(void(*procesa_suscripcion)(void));
void cola_suscripciones_agregar_suscripcion(t_mensaje* mensaje, int* socket);
#endif
