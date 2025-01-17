#ifndef LOCALIZED_POKEMON_H
#define LOCALIZED_POKEMON_H

#include <commons/collections/list.h>
#include "mensajes.h"
#include "posiciones.h"
#include "pokemon.h"

typedef struct{
	uint32_t tamanio_nombre;
	char* nombre;
	uint32_t cantidadPos;
	t_list* posiciones;
} t_localized_pokemon;

/*
localized_pokemon_create sirve para crear mensajes del tipo LOCALIZED_POKEMON
ingresando el nombre del pokemon, la cantidad de posiciones y la lista de posiciones la cual
debera ser creada con anterioridad al llamado de este metodo.
ej: t_localized_pokemon* localized_mensaje = localized_pokemon_create("pikachu",2,posiciones);
posiciones debe ser el tipo t_posicion* es decir una lista de posiciones la cual se creara 
utilizando las funciones incluidas en posiciones.h
*/
t_localized_pokemon* localized_pokemon_create(char* nombre, t_list* posiciones);

/*
localized_pokemon_codigo te retorna el codigo de operacion cuando recibe un localized pokemon
*/
op_code localized_pokemon_codigo(t_localized_pokemon* localized_pokemon);

/*
localized_pokemon_to_buffer convierte un mensaje del tipo localized pokemon en un buffer
*/
t_buffer* localized_pokemon_to_buffer(t_localized_pokemon* localized_pokemon);

/*
localized_pokemon_from_buffer convierte un buffer en un mensaje del tipo localized pokemon
*/
t_localized_pokemon* localized_pokemon_from_buffer(t_buffer* buffer);

/*
localized_pokemon_mostrar dado un mensaje localized pokemon este metodo lo imprime por consola
*/
void localized_pokemon_mostrar(t_localized_pokemon* localized_pokemon);

/*
localized_pokemon_to_string convierte el mensaje localized_pokemon en un string
*/
char* localized_pokemon_to_string(t_localized_pokemon* localized_pokemon);

/*
localized_pokemon_destroy libera la memoria utilizada para crear el mensaje localized pokemon
*/
void localized_pokemon_destroy(t_localized_pokemon* localized_pokemon);

/*
localized_pokemon_size devuelve la cantidad de bytes que ocupada un localized_pokemon especifico
*/
int localized_pokemon_size(t_localized_pokemon* localized);

/*
 * localized_pokemon_get_list devuelve una lista de t_pokemon
 */
t_list* localized_pokemon_get_list(t_localized_pokemon* localized_pokemon);

void* localized_pokemon_to_stream(t_localized_pokemon* localized_pokemon);

t_localized_pokemon* localized_pokemon_from_stream(void* stream);

#endif
