#ifndef PROCESO_TEAM_H
#define PROCESO_TEAM_H

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<commons/collections/list.h>
#include<mensajes/server.h>
#include<mensajes/mensajes.h>
#include<mensajes/localized_pokemon.h>
#include<mensajes/appeared_pokemon.h>
#include<mensajes/get_pokemon.h>
#include<mensajes/catch_pokemon.h>
#include<mensajes/caught_pokemon.h>
#include<mensajes/pokemon.h>
#include<mensajes/suscripcion.h>
#include<mensajes/client.h>
#include<commons/collections/dictionary.h>

#include "variables_globales.h"
#include "entrenador.h"
#include "objetivos.h"
#include "planificador.h"
#include "planificacion.h"


int socket_appeared;
int socket_caught;
int socket_localized;


void terminar_programa(t_log* logger, t_config* config);
t_config* leer_config(void);
t_log* iniciar_logger(char*);

void enviar_get_objetivo(t_list* objetivo_global);
void iniciar_conexion_broker();
int iniciar_suscripcion_broker(op_code cola);

void recibir_mensaje_appeared(int socket_appeared);
void recibir_mensaje_caught(int socket_caught);
void recibir_mensaje_localized(int socket_localized);

void iniciar_puerto_de_escucha();
void procesar_mensajes_directos(int* socket);

void procesar_localized(t_localized_pokemon* localized_pokemon);
void procesar_appeared(t_appeared_pokemon* appeared_pokemon);
void procesar_caught(t_caught_pokemon* caught_pokemon, int32_t id_correlativo);

void test();

#endif

