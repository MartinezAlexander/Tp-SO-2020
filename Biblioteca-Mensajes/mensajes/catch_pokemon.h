#ifndef CATCH_POKEMON_H
#define CATCH_POKEMON_H

#include "mensajes.h"
#include "posiciones.h"

typedef struct{
    uint32_t tamanio_nombre;
    char* nombre;
    t_posicion posicion;
} t_catch_pokemon;

/*
catch_pokemon_create sirve para crear mensajes del tipo CATCH_POKEMON
ingresando el nombre del pokemon la posicion (x,y) donde se encuentra
ej: t_catch_pokemon* catch_mensaje = catch_pokemon_create("pikachu",4,3);
*/
t_catch_pokemon* catch_pokemon_create(char* nombre, uint32_t posX, uint32_t posY);

/*
catch_pokemon_codigo te retorna el codigo de operacion cuando recibe un catch pokemon
*/
op_code catch_pokemon_codigo(t_catch_pokemon* catch_pokemon);

/*
catch_pokemon_to_buffer convierte un mensaje del tipo catch pokemon en un buffer
*/
t_buffer* catch_pokemon_to_buffer(t_catch_pokemon* catch_pokemon);

/*
catch_pokemon_from_buffer convierte un buffer en un mensaje del tipo catch pokemon
*/
t_catch_pokemon* catch_pokemon_from_buffer(t_buffer* buffer);

/*
catch_pokemon_mostrar dado un mensaje catch pokemon este metodo lo imprime por consola
*/
void catch_pokemon_mostrar(t_catch_pokemon* catch_pokemon);

/*
catch_pokemon_to_string convierte un mensaje catch pokemon en un string
*/
char* catch_pokemon_to_string(t_catch_pokemon*catch_pokemon);

/*
catch_pokemon_destroy libera la memoria utilizada para crear el mensaje catch pokemon
*/
void catch_pokemon_destroy(t_catch_pokemon* catch_pokemon);

#endif
