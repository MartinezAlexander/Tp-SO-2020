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
void crear_block(int numero);
t_config* obtener_bloque_por_indice(int numero_bloque);
int bloque_esta_libre(int numero_bloque);
int obtener_tamanio_ocupado_por_bloque(int numero_bloque);
int bytes_libres_bloque(int numero_bloque);
int entra_en_bloque(t_posicion posicion,int cantidad, int numero_bloque);

void agregar_pokemon_a_bloque(int bloque, t_posicion posicion, int cantidad);
void agregar_nuevo_pokemon_a_bloque(int bloque, t_posicion posicion, int cantidad);

int obtener_primer_bloque_con_espacio(char** bloques, t_posicion posicion, int cantidad);

int obtener_bloque_con_posicion(char** bloques, t_posicion posicion, int cantidad);
char* posicion_to_key(t_posicion posicion);

int eliminar_pokemon_de_bloque(int bloque, t_posicion posicion);
t_list* obtener_posiciones_de_bloque(int bloque);

int obtener_tamanio_listado_de_bloques(char** bloques);

char* obtener_string_bloques_sin(char** bloques_array, int bloque_a_sacar);

#endif /* BLOCK_H_ */
