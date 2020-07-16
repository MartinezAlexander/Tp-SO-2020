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
#include <commons/config.h>
#include <commons/string.h>
#include "info_posiciones.h"

typedef struct{
	char* path;
	uint32_t numero;
	FILE* datos;
}block;

t_list* bloques;

char* crear_nombre_bloque_numero(int numero);
char* crear_directorio_bloque();
void crear_block(int numero);
char* leer_bloque(int bloque);

t_list* obtener_posiciones_de_bloques(char** bloques);

int array_cantidad_de_elementos(char** array) ;
char* obtener_path_bloque_de_lista(char** bloques,int indice_numero_bloque);
void vaciar_bloques(char** bloques);
int actualizar_bloques(char** bloques, t_list* posiciones);

#endif /* BLOCK_H_ */
