/*
 * proceso-game-boy.h
 *
 *  Created on: 14 may. 2020
 *      Author: utnso
 */

#ifndef PROCESO_GAME_BOY_H_
#define PROCESO_GAME_BOY_H_

#include <stdio.h>
#include <stdlib.h>
#include <mensajes/client.h>
#include <mensajes/mensajes.h>
#include <mensajes/appeared_pokemon.h>
#include <mensajes/new_pokemon.h>
#include <mensajes/get_pokemon.h>
#include <mensajes/catch_pokemon.h>
#include <mensajes/caught_pokemon.h>
#include <mensajes/localized_pokemon.h>
#include <mensajes/suscripcion.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>

t_config* config;
t_log* logger;

char* ip_broker;
char* ip_team;
char* ip_gamecard;
char* puerto_broker;
char* puerto_team;
char* puerto_gamecard;

/*
 uint32_t puerto_broker;
 uint32_t puerto_team;
 uint32_t puerto_gamecard;
 */

t_proceso obtener_id_proceso(char* id);

op_code obtener_tipo_mensaje(char* tipo);

uint32_t atrapo_pokemon(char* confirmacion);

t_mensaje* procesar_mensaje(char** mensaje, op_code codigo, t_proceso id);

void enviar_a(t_proceso id, t_mensaje* mensaje);

void inicializar_variables();

t_log* iniciar_logger(char* path);

t_config* leer_config(void);

void terminar_programa(t_log* logger, t_config* config);

#endif /* PROCESO_GAME_BOY_H_ */