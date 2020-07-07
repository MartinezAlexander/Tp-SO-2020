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

typedef struct{
	t_bitarray* bitarray;
	FILE* file;
}bitmap;

bitmap* estado_bloques;

void ocupar_bloque(int nuevo_bloque);
int obtener_bloque_disponible();

#endif /* BITMAP_H_ */
