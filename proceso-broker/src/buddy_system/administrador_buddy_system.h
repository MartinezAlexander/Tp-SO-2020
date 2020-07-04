#ifndef BUDDY_SYSTEM_ADMINISTRADOR_BUDDY_SYSTEM_H_
#define BUDDY_SYSTEM_ADMINISTRADOR_BUDDY_SYSTEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <mensajes/mensajes.h>
#include <pthread.h>
#include "../memoria_cache.h"
#include "buddy_system.h"
#include "../logger.h"

t_buddy* buddy_principal;
pthread_mutex_t mutex_bs;

void iniciar_buddy_system();
t_list* obtener_mensajes_cacheados_por_cola_bs(op_code cola);
void bs_cachear_mensaje(t_mensaje* mensaje);
void fifo();
void lru_actualizar(t_buddy* buddy);
void lru();
int ff(t_mensaje* mensaje);
int bf(t_mensaje* mensaje);
void proc_para_almacenamiento_de_datos(t_mensaje* mensaje, int(*algoritmo)(t_mensaje* mensaje), void(*eliminar)(void));
void cachear_mensaje_bs(t_mensaje* mensaje);
char* buddy_to_string(t_buddy* buddy);
//int es_hora_de_compactar(int tamanio_a_guardar);


#endif /* BUDDY_SYSTEM_ADMINISTRADOR_BUDDY_SYSTEM_H_ */
