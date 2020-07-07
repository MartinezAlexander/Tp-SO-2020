/*
 * file_pokemon.h
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#ifndef FILE_POKEMON_H_
#define FILE_POKEMON_H_

#include "tall_grass.h"
#include "block.h"

typedef struct{
	char* path;
	char* especie;
	FILE* metadata;
	int open;
}pokemon_file;

t_list* pokemons;

pokemon_file* pokemon_file_create(char* especie);

pokemon_file* existe_pokemon(char* especie);
pokemon_file* obtener_pokemon(char* especie);
void agregar_pokemon(pokemon_file* archivo, t_posicion posicion, int cantidad);

void incrementar_cantidad(pokemon_file* archivo, t_posicion posicion);
int existe_posicion(pokemon_file* archivo, t_posicion pos);

void abrir_archivo(pokemon_file* archivo);
void cerrar_archivo(pokemon_file* archivo);

//TODO cuando creo un archivo, ya creo un bloque aunque este vacio
FILE* crear_metadata(char* ruta);

#endif /* FILE_POKEMON_H_ */
