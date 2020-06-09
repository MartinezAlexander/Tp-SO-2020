/*
 * procesamiento_appeared.h
 *
 *  Created on: 16 may. 2020
 *      Author: utnso
 */

#ifndef PROCESAMIENTO_APPEARED_H_
#define PROCESAMIENTO_APPEARED_H_

#include<stdio.h>
#include<stdlib.h>
#include<mensajes/appeared_pokemon.h>
#include <pthread.h>
#include "../objetivos.h"
#include "../variables_globales.h"
#include "../planificacion/planificacion.h"

void descartar_pokemon(t_pokemon* pokemon);
void procesar_appeared(t_appeared_pokemon* appeared_pokemon);
int procesar_pokemon(t_pokemon* pokemon);

#endif /* PROCESAMIENTO_APPEARED_H_ */
