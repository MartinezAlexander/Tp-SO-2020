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
#include <mensajes/posiciones.h>
#include <commons/string.h>

typedef struct{
	char* path;
	uint32_t numero;
	FILE* datos;
}block;

t_list* bloques;

char* crear_nombre_bloque_numero(int numero);
char* crear_directorio_bloque();
t_config* crear_block(int numero);
t_config* obtener_bloque_por_indice(int numero_bloque);
int bloque_esta_libre(int numero_bloque);
int obtener_tamanio_ocupado_por_bloque(int numero_bloque);
int bytes_libres_bloque(int numero_bloque);
int entra_en_bloque(t_posicion posicion,int cantidad, int numero_bloque);

#endif /* BLOCK_H_ */
