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

t_dictionary* diccionario_especies_recibidas;
pthread_mutex_t mutex_diccionario_especies;

t_queue* cola_pokemones_en_espera;

//Estadisticas
t_dictionary* diccionario_ciclos_entrenador;
int cambios_de_contexto;

int finalizo_team;

#endif /* VARIABLES_GLOBALES_H_ */
