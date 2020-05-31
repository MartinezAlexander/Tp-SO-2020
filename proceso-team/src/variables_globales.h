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
#include<commons/log.h>
#include<commons/string.h>

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

//Estadisticas
t_dictionary* diccionario_ciclos_entrenador;

int finalizo_team;

#endif /* VARIABLES_GLOBALES_H_ */
