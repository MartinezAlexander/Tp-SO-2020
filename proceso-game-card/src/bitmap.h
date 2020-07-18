/*
 * bitmap.h
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdio.h>
#include <commons/bitarray.h>
#include "variables_globales.h"
#include "tall_grass.h"
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct{
	t_bitarray* bitarray;
	char* path;
}bitmap;

bitmap* estado_bloques;

void bitmap_cargar();
void ocupar_bloque(int nuevo_bloque);
void liberar_bloque(int numero_bloque);
int obtener_bloque_disponible();



#endif /* BITMAP_H_ */
