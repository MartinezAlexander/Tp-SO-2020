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

#include "entrenador.h"


t_list* obtener_objetivo_global(t_list* entrenadores);
int cantidad_de_elementos(char** array);

void terminar_programa(t_log* logger, t_config* config);
t_config* leer_config(void);
t_log* iniciar_logger(char*);

void enviar_get_objetivo(t_list* objetivo_global);
void iniciar_conexion_broker(char* ip, char* puerto);
void iniciar_puerto_de_escucha();

void procesar_mensajes(int* socket);
void procesar_localized(t_localized_pokemon* localized_pokemon);
void procesar_appeared(t_appeared_pokemon* appeared_pokemon);
void procesar_caught(t_caught_pokemon* caught_pokemon);
int cumplio_objetivo_global();
int pokemon_dentro_de_objetivos(char* pokemon);

#endif

