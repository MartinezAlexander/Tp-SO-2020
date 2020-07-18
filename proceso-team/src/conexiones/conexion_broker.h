/*
 * conexion_broker.h
 *
 *  Created on: 16 may. 2020
 *      Author: utnso
 */

#ifndef CONEXION_BROKER_H_
#define CONEXION_BROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<commons/string.h>
#include<commons/collections/list.h>
#include<mensajes/mensajes.h>
#include<mensajes/suscripcion.h>
#include<mensajes/client.h>
#include<mensajes/localized_pokemon.h>
#include<mensajes/appeared_pokemon.h>
#include<mensajes/caught_pokemon.h>
#include<mensajes/get_pokemon.h>


#include "../mensajes/procesamiento_caught.h"
#include "../mensajes/procesamiento_appeared.h"
#include "../mensajes/procesamiento_localized.h"
#include "../utils/log_utils.h"
#include "../variables_globales.h"

pthread_t hilo_appeared;
pthread_t hilo_caught;
pthread_t hilo_localized;
pthread_t hilo_reconexion;
sem_t semaforo_reconexion;
pthread_mutex_t mutex_reconexion;
int estoy_reconectando;

int socket_appeared;
int socket_caught;
int socket_localized;
sem_t semaforo_procesamiento_appeared;
sem_t semaforo_procesamiento_caught;
sem_t semaforo_procesamiento_localized;

void iniciar_conexion_broker();
void iniciar_suscripcion_broker(op_code cola);

void reconectar_al_broker();
void iniciar_reconexion();

void suscribirse_a_cola(int* socket, op_code cola);
void recibir_appeared();
void recibir_caught();
void recibir_localized();

void enviar_get_objetivo(t_list* objetivo_global);

void conexion_broker_destroy();

#endif /* CONEXION_BROKER_H_ */
