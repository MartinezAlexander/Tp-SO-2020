#include "pokemon.h"
#include <string.h>

t_pokemon* pokemon_create(char* especie, t_posicion posicion){
	t_pokemon* pokemon = malloc( sizeof(t_pokemon) );
	pokemon->especie =  especie;
	pokemon->posicion = posicion;
	return pokemon;
}

int pokemon_size(t_pokemon* pokemon){
	return strlen(pokemon->especie) + 1 + posicion_size(pokemon->posicion) ;
}

void pokemon_destroy(t_pokemon* pokemon){
	//TODO Se libera la memoria del char* especie?
	free(pokemon);
}
