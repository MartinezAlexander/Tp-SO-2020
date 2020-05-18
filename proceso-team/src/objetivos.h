/*
 * objetivos.h
 *
 *  Created on: 6 may. 2020
 *      Author: utnso
 */

#ifndef OBJETIVOS_H_
#define OBJETIVOS_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/collections/list.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "entrenador.h"

t_list* obtener_objetivo_global(t_list* entrenadores);
int pokemon_dentro_de_objetivos(t_list* objetivos, char* pokemon);
int cumplio_objetivo_global(t_list* objetivo_global, t_list* entrenadores);

#endif /* OBJETIVOS_H_ */
