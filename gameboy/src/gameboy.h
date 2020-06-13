/*
 * gameboy.h
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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
#include <commons/log.h>
#include <commons/config.h>
#include "logger.h"

char* ip_broker;
char* ip_team;
char* ip_gamecard;
char* puerto_broker;
char* puerto_team;
char* puerto_gamecard;
char* path_logger;

int32_t tiempo_conexion;
int32_t hay_tiempo = 1;

t_config* config;
t_proceso obtener_id_proceso(char* id);
t_mensaje* procesar_mensaje(char** mensaje, op_code codigo, t_proceso id);
t_config* leer_config(void);
op_code obtener_tipo_mensaje(char* tipo);
uint32_t atrapo_pokemon(char* confirmacion);

void inicializar_variables();
void enviar_a(t_proceso id, t_mensaje* mensaje);
void desconectar_suscriptor();
void recepcion_mensajes(int socket);
void terminar_programa(t_log* logger, t_config* config);

#endif /* GAMEBOY_H_ */
