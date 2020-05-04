#ifndef APPEARED_POKEMON_H
#define APPEARED_POKEMON_H

#include "mensajes.h"
#include "posiciones.h"

typedef struct{
    uint32_t tamanio_nombre;
    char* nombre;
    t_posicion posicion;
} t_appeared_pokemon;

/*
appeared_pokemon_create sirve para crear mensajes del tipo APPEARED_POKEMON
ingresando el nombre del pokemon la posicion (x,y) donde se encuentra
ej: t:appeared_pokemon* appeared_mensaje = appeared_pokemon_create("pikachu", 2, 3);
*/
t_appeared_pokemon* appeared_pokemon_create(char* nombre, uint32_t posX, uint32_t posY);

/*
appeared_pokemon_codigo te retorna el codigo de operacion cuando recibe un appeared pokemon
*/
op_code appeared_pokemon_codigo(t_appeared_pokemon* appeared_pokemon);

/*
appeared_pokemon_to_buffer convierte un mensaje del tipo appeared pokemon en un buffer
*/
t_buffer* appeared_pokemon_to_buffer(t_appeared_pokemon* appeared_pokemon);

/*
appeared_pokemon_from_buffer convierte un buffer en un mensaje del tipo appeared pokemon
*/
t_appeared_pokemon* appeared_pokemon_from_buffer(t_buffer* buffer);

/*
appeared_pokemon_mostrar dado un mensaje appeared pokemon este metodo lo imprime por consola
*/
void appeared_pokemon_mostrar(t_appeared_pokemon* appeared_pokemon);

/*
appeared_pokemon_to_string convierte un mensaje appeared pokemon en un string
*/
char* appeared_pokemon_to_string(t_appeared_pokemon* appeared_pokemon);

/*
appeared_pokemon_destroy libera la memoria utilizada para crear el mensaje appeared pokemon
*/
void appeared_pokemon_destroy(t_appeared_pokemon* appeared_pokemon);

#endif
