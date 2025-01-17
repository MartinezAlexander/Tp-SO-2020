#ifndef PROCESO_TEAM_H
#define PROCESO_TEAM_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<commons/collections/list.h>
#include <pthread.h>

#include "variables_globales.h"
#include "entrenador.h"
#include "objetivos.h"
#include "planificacion/planificador.h"
#include "conexiones/conexion_gameboy.h"
#include "conexiones/conexion_broker.h"
//Includes necesarios para el test manual que hay

#include "planificacion/planificacion.h"
#include<mensajes/appeared_pokemon.h>
#include<mensajes/mensajes.h>
#include<mensajes/pokemon.h>
#include"tests/test_planificacion.h"
#include"tests/test_deadlock.h"

void terminar_programa(t_log* logger, t_config* config);
t_config* leer_config(char* path_config);
t_log* iniciar_logger(char*);
void inicializar_variables();

void test();

#endif

