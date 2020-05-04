#ifndef CAUGHT_POKEMON_H
#define CAUGHT_POKEMON_H

#include "mensajes.h"

typedef struct{
    uint32_t atrapado;
} t_caught_pokemon;

/*
caught_pokemon_create sirve para crear mensajes del tipo CAUGHT_POKEMON
ingresando 0 si no fue atrapado o 1 si fue atrapado
ej: t_caught_pokemon* caught_mensaje = caught_pokemon_create(0);
*/
t_caught_pokemon* caught_pokemon_create(uint32_t fueAtrapado);

/*
caught_pokemon_codigo te retorna el codigo de operacion cuando recibe un caught pokemon
*/
op_code caught_pokemon_codigo(t_caught_pokemon* caught_pokemon);

/*
caught_pokemon_to_buffer convierte un mensaje del tipo caught pokemon en un buffer
*/
t_buffer* caught_pokemon_to_buffer(t_caught_pokemon* caught_pokemon);

/*
caught_pokemon_from_buffer convierte un buffer en un mensaje del tipo caught pokemon
*/
t_caught_pokemon* caught_pokemon_from_buffer(t_buffer* buffer);

/*
caught_pokemon_mostrar dado un mensaje caught pokemon este metodo lo imprime por consola
*/
void caught_pokemon_mostrar(t_caught_pokemon* caught_pokemon);

/*
caught_pokemon_to_string convierte un mensaje caught_pokemon en un string
*/
char* caught_pokemon_to_string(t_caught_pokemon* caught_pokemon);

/*
caught_pokemon_destroy libera la memoria utilizada para crear el mensaje caught pokemon
*/
void caught_pokemon_destroy(t_caught_pokemon* caught_pokemon);

#endif
