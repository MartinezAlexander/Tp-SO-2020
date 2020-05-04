#ifndef NEW_POKEMON_H
#define NEW_POKEMON_H

#include "mensajes.h"
#include "posiciones.h"

typedef struct{
    uint32_t tamanio_nombre;
    char* nombre;
    t_posicion posicion;
    uint32_t cantidad;
} t_new_pokemon;

/*
new_pokemon_create sirve para crear mensajes del tipo NEW_POKEMON
ingresando el nombre del pokemon, la posicion (x,y) donde se encuentra 
y la cantidad de pokemones de ese tipo que hay en esa posicion.
ej: t_new_pokemon* new_mensaje = new_pokemon_create("pikachu",2,3,5);
*/
t_new_pokemon* new_pokemon_create(char* nombre, uint32_t posX, uint32_t posY, uint32_t cantidad);

/*
new_pokemon_codigo te retorna el codigo de operacion cuando recibe un new pokemon
*/
op_code new_pokemon_codigo(t_new_pokemon* new_pokemon);

/*
new_pokemon_to_buffer convierte un mensaje del tipo new pokemon en un buffer
*/
t_buffer* new_pokemon_to_buffer(t_new_pokemon* new_pokemon);

/*
new_pokemon_from_buffer convierte un buffer en un mensaje del tipo new pokemon
*/
t_new_pokemon* new_pokemon_from_buffer(t_buffer* buffer);

/*
new_pokemon_mostrar dado un mensaje new pokemon este metodo lo imprime por consola
*/
void new_pokemon_mostrar(t_new_pokemon* new_pokemon);

/*
new_pokemon_to_string convierte el mensaje new_pokemon en un string
*/
char* new_pokemon_to_string(t_new_pokemon* new_pokemon);

/*
new_pokemon_destroy libera la memoria utilizada para crear el mensaje new pokemon
*/
void new_pokemon_destroy(t_new_pokemon* new_pokemon);

#endif
