#ifndef GET_POKEMON_H
#define GET_POKEMON_H

#include "mensajes.h"

typedef struct{
    uint32_t tamanio_nombre;
    char* nombre;
} t_get_pokemon;

/*
get_pokemon_create sirve para crear mensajes del tipo GET_POKEMON
ingresando el nombre del pokemon a obtener
ej: t_get_pokemon* get_mensaje = get_pokemon_create("pikachu");
*/
t_get_pokemon* get_pokemon_create(char* nombre);

/*
get_pokemon_codigo te retorna el codigo de operacion cuando recibe un get pokemon
*/
op_code get_pokemon_codigo(t_get_pokemon* get_pokemon);

/*
get_pokemon_to_buffer convierte un mensaje del tipo get pokemon en un buffer
*/
t_buffer* get_pokemon_to_buffer(t_get_pokemon* get_pokemon);

/*
get_pokemon_from_buffer convierte un buffer en un mensaje del tipo get pokemon
*/
t_get_pokemon* get_pokemon_from_buffer(t_buffer* buffer);

/*
get_pokemon_mostrar dado un mensaje get pokemon este metodo lo imprime por consola
*/
void get_pokemon_mostrar(t_get_pokemon* get_pokemon);

/*
get_pokemon_to_string convierte en un string al mensaje get_pokemon
*/
char* get_pokemon_to_string(t_get_pokemon* get_pokemon);

/*
get_pokemon_destroy libera la memoria utilizada para crear el mensaje get pokemon
*/
void get_pokemon_destroy(t_get_pokemon* get_pokemon);

#endif
