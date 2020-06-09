/*
 * procesamiento_localized.h
 *
 *  Created on: 16 may. 2020
 *      Author: utnso
 */

#ifndef PROCESAMIENTO_LOCALIZED_H_
#define PROCESAMIENTO_LOCALIZED_H_

#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include<mensajes/mensajes.h>
#include<mensajes/localized_pokemon.h>

#include "procesamiento_appeared.h"
#include "../objetivos.h"

void procesar_localized(t_localized_pokemon* localized_pokemon);
int especie_no_procesada(char* especie);

#endif /* PROCESAMIENTO_LOCALIZED_H_ */
