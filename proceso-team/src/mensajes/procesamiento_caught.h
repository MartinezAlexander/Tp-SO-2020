/*
 * procesamiento_caught.h
 *
 *  Created on: 16 may. 2020
 *      Author: utnso
 */

#ifndef PROCESAMIENTO_CAUGHT_H_
#define PROCESAMIENTO_CAUGHT_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/collections/dictionary.h>
#include<commons/string.h>
#include<mensajes/caught_pokemon.h>
#include "../entrenador.h"
#include "../planificacion/planificacion.h"


void descartar_caught(t_caught_pokemon* mensaje);
void procesar_caught(t_caught_pokemon* caught_pokemon, int32_t id_correlativo);

#endif /* PROCESAMIENTO_CAUGHT_H_ */
