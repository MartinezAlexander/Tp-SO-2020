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
#include <mensajes/mensajes.h>

void ejecutar_new(t_new_pokemon* pokemon, int id);
void ejecutar_catch(t_catch_pokemon* pokemon, int id);
void ejecutar_get(t_get_pokemon* pokemon, int id);

#endif /* PROCESAMIENTO_MENSAJES_H_ */
