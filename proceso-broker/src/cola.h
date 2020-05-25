/*
 * cola.h
 *
 *  Created on: 8 may. 2020
 *      Author: utnso
 */

#ifndef COLA_H_
#define COLA_H_


#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include <mensajes/mensajes.h>
#include "suscriptor.h"
#include <semaphore.h>

//estructura de cola con sus respectivos suscriptores
typedef struct{
	t_queue* queue;
	t_list* suscriptores;
	pthread_t hilo;
	sem_t semaforoMensajes;
	sem_t semaforoSuscriptores;
}t_cola_mensajeria;

t_cola_mensajeria* cola_mensajeria_new;
t_cola_mensajeria* cola_mensajeria_appeared;
t_cola_mensajeria* cola_mensajeria_get;
t_cola_mensajeria* cola_mensajeria_localized;
t_cola_mensajeria* cola_mensajeria_catch;
t_cola_mensajeria* cola_mensajeria_caught;

t_cola_mensajeria* cola_mensajeria_create();
t_cola_mensajeria* cola_mensajeria_obtener(op_code codigo);
void cola_mensajeria_recibir_mensaje(t_cola_mensajeria* cola, t_mensaje* mensaje, int* ultimo_id);


#endif /* COLA_H_ */
