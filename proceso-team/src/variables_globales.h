/*
 * variabales_globales.h
 *
 *  Created on: 14 may. 2020
 *      Author: utnso
 */

#ifndef VARIABLES_GLOBALES_H_
#define VARIABLES_GLOBALES_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include<commons/collections/queue.h>
#include<commons/log.h>
#include<commons/string.h>
#include<semaphore.h>

//Controla el hilo de planificacion. Sirve para poder tener el ciclo: entrenador -> planificador
sem_t semaforo_planificacion;

t_config* config;

t_list* objetivo_global;

uint32_t tiempo_de_reconexion;

char* ip_broker;
char* puerto_broker;

char* ip_team;
char* puerto_team;

int32_t team_id;

t_log* logger;

//Aca guardo las especies que recibi con un appeared o localized.
//Sirve para que el localized pueda saber si ya procese mensajes de una especie
t_dictionary* diccionario_especies_recibidas;
pthread_mutex_t mutex_diccionario_especies;

//Aca tengo todos los pokemones que no pude procesar en el momento que llegaron
//ya sea porque no habia entrenadores libres o porque la especie ya esta siendo procesada
//y me quiero guardar un pokemon de repuesto en caso de que falle el catch
t_queue* cola_pokemones_en_espera;
pthread_mutex_t mutex_cola_espera;

//Utilizado para garantizar mutua exclusion en el procesamiento de un pokemon.
//Ya que puedo procesar por nuevo mensaje o por cola de espera en hilos simultaneos
pthread_mutex_t mutex_procesamiento_pokemon;

//Para llevar las cuentas de cuantos ciclos realizo cada entrenador
t_dictionary* diccionario_ciclos_entrenador;
int cambios_de_contexto;

int finalizo_team;

#endif /* VARIABLES_GLOBALES_H_ */
