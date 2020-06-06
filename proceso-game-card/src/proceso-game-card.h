/*
 * proceso-game-card.h
 *
 *  Created on: 6 jun. 2020
 *      Author: utnso
 */

#ifndef PROCESO_GAME_CARD_H_
#define PROCESO_GAME_CARD_H_

#include <stdio.h>
#include <stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>

t_log* logger;
t_config* config;

int tiempo_reintento_conexion;
int tiempo_reintento_operacion;
int tiempo_retardo_operacion;
char* punto_de_montaje_tallgrass;
char* ip_broker;
char* puerto_broker;

void inicializar_variables_globales();
t_log* iniciar_logger(char* path);
t_config* leer_config(void);

void inicializar_filesystem();
void iniciar_conexion_broker();
void iniciar_puerto_escucha();

void terminar_programa();

#endif /* PROCESO_GAME_CARD_H_ */
