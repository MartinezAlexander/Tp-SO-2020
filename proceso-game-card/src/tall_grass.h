/*
 * tall_grass.h
 *
 *  Created on: 6 jun. 2020
 *      Author: utnso
 */

#ifndef TALL_GRASS_H_
#define TALL_GRASS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/collections/list.h>
#include <commons/txt.h>
#include <commons/config.h>
#include <mensajes/posiciones.h>

#include "variables_globales.h"

FILE* crear_archivo(char* directorio, char* archivo);
void crear_directorio(char* directorio);
char* path(char* direccion, char* direccion2);

void metadata_cargar();

#endif /* TALL_GRASS_H_ */
