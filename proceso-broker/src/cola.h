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

//estructura de cola con sus respectivos suscriptores
typedef struct{
	t_queue* queue;
	t_list* suscriptores;
	pthread_t hilo;
	//semaforo
}t_cola_mensajeria;

typedef struct{
	int socket;
	int32_t pid;
}t_suscriptor;

t_suscriptor* suscriptor_create(int socket, int32_t pid);
t_cola_mensajeria* cola_mensajeria_create();


#endif /* COLA_H_ */
