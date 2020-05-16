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
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<commons/collections/list.h>

t_config* config;

t_list* objetivo_global;

uint32_t tiempo_de_reconexion;

char* ip_broker;
char* puerto_broker;

t_log* logger;

#endif /* VARIABLES_GLOBALES_H_ */
