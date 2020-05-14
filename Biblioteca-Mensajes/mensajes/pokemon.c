#include "pokemon.h"

t_pokemon* pokemon_create(char* especie, t_posicion posicion){
	t_pokemon* pokemon = malloc( sizeof(t_pokemon) );
	pokemon->especie =  especie;
	pokemon->posicion = posicion;
	return pokemon;
}

void pokemon_destroy(t_pokemon* pokemon){
	free(pokemon);
}
