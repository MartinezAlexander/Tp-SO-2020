/*
 * block.h
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include "bitmap.h"
#include "variables_globales.h"
#include <stdint.h>
#include <commons/collections/list.h>

typedef struct{
	char* path;
	uint32_t numero;
	FILE* datos;
}block;

t_list* bloques;

void crear_block(int numero);

void agregar_pokemon_a_bloque(int bloque, t_posicion posicion, int cantidad);
void agregar_nuevo_pokemon_a_bloque(int bloque, t_posicion posicion, int cantidad);

int obtener_primer_bloque_con_espacio(char** bloques, t_posicion posicion, int cantidad);

int obtener_bloque_con_posicion(char** bloques, t_posicion posicion, int cantidad);
char* posicion_to_key(t_posicion posicion);

#endif /* BLOCK_H_ */
