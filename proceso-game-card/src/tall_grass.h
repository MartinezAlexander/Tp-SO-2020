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
#include <commons/bitarray.h>

#include "variables_globales.h"

typedef struct{
	char* path;
	char* especie;
	FILE* metadata;
	int open;
}pokemon_file;

typedef struct{
	char* path;
	uint32_t numero;
	FILE* datos;
}block;

typedef struct{
	t_bitarray* bitarray;
	FILE* file;
}bitmap;

t_list* pokemons;
t_list* bloques;
bitmap* estado_bloques;
int blocks;
int block_size;

pokemon_file* pokemon_file_create(char* especie);
void crear_block(int numero);

void ocupar_bloque(int nuevo_bloque);
int obtener_bloque_disponible();

pokemon_file* existe_pokemon(char* especie);
pokemon_file* obtener_pokemon(char* especie);
void agregar_pokemon(pokemon_file* archivo, t_posicion posicion, int cantidad);

void abrir_archivo(pokemon_file* archivo);
void cerrar_archivo(pokemon_file* archivo);

FILE* crear_archivo(char* directorio, char* archivo);
void crear_directorio(char* directorio);
char* path(char* direccion, char* direccion2);
FILE* crear_metadata(char* ruta);

#endif /* TALL_GRASS_H_ */
