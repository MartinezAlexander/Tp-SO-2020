/*
 * procesamiento_mensajes.h
 *
 *  Created on: 6 jun. 2020
 *      Author: utnso
 */

#ifndef PROCESAMIENTO_MENSAJES_H_
#define PROCESAMIENTO_MENSAJES_H_

#include <stdio.h>
#include <stdlib.h>
#include <mensajes/client.h>
#include <mensajes/mensajes.h>
#include <mensajes/new_pokemon.h>
#include <mensajes/catch_pokemon.h>
#include <mensajes/get_pokemon.h>
#include <mensajes/appeared_pokemon.h>

#include "tall_grass.h"
#include "variables_globales.h"

void ejecutar_new(t_new_pokemon* pokemon, int id);
void ejecutar_catch(t_catch_pokemon* pokemon, int id);
void ejecutar_get(t_get_pokemon* pokemon, int id);

#endif /* PROCESAMIENTO_MENSAJES_H_ */
