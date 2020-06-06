/*
 * variables_globales.h
 *
 *  Created on: 6 jun. 2020
 *      Author: utnso
 */

#ifndef VARIABLES_GLOBALES_H_
#define VARIABLES_GLOBALES_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

t_log* logger;
t_config* config;

int tiempo_reintento_conexion;
int tiempo_reintento_operacion;
int tiempo_retardo_operacion;
char* punto_de_montaje_tallgrass;
char* ip_broker;
char* puerto_broker;

int card_id;

#endif /* VARIABLES_GLOBALES_H_ */
