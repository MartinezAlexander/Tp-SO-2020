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

pthread_mutex_t mutex_modificacion_de_archivo;

char* obtener_pokemon(char* especie);
char* pokemon_file_create(char* especie);

void agregar_pokemon(char* archivo, t_posicion posicion, int cantidad);

t_list* obtener_posiciones_pokemon(char* archivo);
void decrementar_cantidad(char* archivo, t_posicion posicion);
int existe_posicion(char* archivo, t_posicion posicion);

void abrir_archivo(char* archivo);
void cerrar_archivo(char* archivo);

void crear_metadata(char* ruta);

#endif /* FILE_POKEMON_H_ */
