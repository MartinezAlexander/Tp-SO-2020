/*
 * file_pokemon.h
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#ifndef FILE_POKEMON_H_
#define FILE_POKEMON_H_

#include <pthread.h>
#include "tall_grass.h"
#include "block.h"
#include "logger.h"

pthread_mutex_t mutex_modificacion_de_archivo;

typedef struct{
	char* especie;
	t_config* file;
	char* path;
	pthread_mutex_t mutex_file;
}file_pokemon;

t_list* pokemones_files;

file_pokemon* file_pokemon_obtener(char* especie);
void abrir_archivo_dos(file_pokemon* poke_file, int hilo);
void agregar_pokemon_dos(file_pokemon* poke_file, t_posicion posicion, int cantidad);
void cerrar_archivo_dos(file_pokemon* poke_file);
int decrementar_cantidad_dos(file_pokemon* poke_file, t_posicion posicion);
t_list* obtener_posiciones_pokemon_dos(file_pokemon* poke_file);


char* obtener_pokemon(char* especie);
char* pokemon_file_create(char* especie);

void agregar_pokemon(file_pokemon* poke_file, t_posicion posicion, int cantidad);

t_list* obtener_posiciones_pokemon(file_pokemon* poke_file);
int decrementar_cantidad(file_pokemon* poke_file, t_posicion posicion);
int existe_posicion(file_pokemon* poke_file, t_posicion posicion);

void abrir_archivo(file_pokemon* poke_file, int hilo);
void cerrar_archivo(file_pokemon* poke_file);

void crear_metadata(char* ruta);

#endif /* FILE_POKEMON_H_ */
