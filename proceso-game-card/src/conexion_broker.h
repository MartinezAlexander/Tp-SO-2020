/*
 * conexion_broker.h
 *
 *  Created on: 6 jun. 2020
 *      Author: utnso
 */

#ifndef CONEXION_BROKER_H_
#define CONEXION_BROKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <mensajes/mensajes.h>
#include <mensajes/client.h>
#include <mensajes/suscripcion.h>
#include <mensajes/catch_pokemon.h>
#include <mensajes/new_pokemon.h>
#include <mensajes/get_pokemon.h>

#include "variables_globales.h"

sem_t semaforo_procesamiento_catch;
sem_t semaforo_procesamiento_get;
sem_t semaforo_procesamiento_new;

pthread_t hilo_catch;
pthread_t hilo_get;
pthread_t hilo_new;

pthread_t hilo_reconexion;
sem_t semaforo_reconexion;
pthread_mutex_t mutex_reconexion;
int estoy_reconectando;

int socket_catch;
int socket_get;
int socket_new;

void iniciar_conexion_broker();
void procesar_cola(op_code cola);
void suscribirse_a_cola(int* socket, op_code cola);

void procesar_catch();
void procesar_get();
void procesar_new();

void iniciar_reconexion();
void reconectar();

#endif /* CONEXION_BROKER_H_ */
