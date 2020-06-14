#ifndef ADMINISTRADOR_PARTICIONES_H_
#define ADMINISTRADOR_PARTICIONES_H_

#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/string.h>
#include <mensajes/mensajes.h>
#include "particiones_dinamicas.h"
#include "../memoria_cache.h"
#include "../suscriptor.h"
#include "../logger.h"
#include <pthread.h>

pthread_mutex_t mutex_cacheo;

t_list* particiones;

t_queue* particiones_victimas;

t_list* particiones_victimas_lru;

int busquedas_fallidas;

void iniciar_administrador_pd();

int first_fit(t_mensaje* mensaje);

int best_fit(t_mensaje* mensaje);

int es_hora_de_compactar();

void fifo_eliminar();

void actualizar_lru(t_particion* particion);

void lru_eliminar();

void combinar_particiones_contiguas_a(t_particion* particion_victima);

void compactar_particiones();

void procedimiento_para_almacenamiento_de_datos(t_mensaje* mensaje, int(*algorimo)(t_mensaje* mensaje), void(*eliminar)(void));

void administrador_cachear_mensaje(t_mensaje* mensaje);

t_list* obtener_mensajes_cacheados_por_cola_pd(op_code cola);

#endif
