/*
 * proceso-team.h
 *
 *  Created on: 1 may. 2020
 *      Author: utnso
 */

#ifndef PROCESO_TEAM_SRC_PROCESO_TEAM_H_
#define PROCESO_TEAM_SRC_PROCESO_TEAM_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<commons/collections/list.h>

#include "entrenador.h"


t_list* obtener_objetivo_global(t_list* entrenadores);
int cantidad_de_elementos(char** array);

void terminar_programa(t_log* logger, t_config* config);
t_config* leer_config(void);
t_log* iniciar_logger(char*);

#endif /* PROCESO_TEAM_SRC_PROCESO_TEAM_H_ */
