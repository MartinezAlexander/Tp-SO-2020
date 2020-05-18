#ifndef POKEMON_H
#define POKEMON_H

#include <stdlib.h>
#include "posiciones.h"

typedef struct{
	char* especie;
	t_posicion posicion;
}t_pokemon;

t_pokemon* pokemon_create(char* especie, t_posicion posicion);
void pokemon_destroy(t_pokemon* pokemon);

#endif
