#include "pokemon.h"
#include <string.h>
#include <commons/string.h>

t_pokemon* pokemon_create(char* especie, t_posicion posicion){
	t_pokemon* pokemon = malloc( sizeof(t_pokemon) );
	pokemon->especie =  string_from_format("%s",especie);
	pokemon->posicion = posicion;
	return pokemon;
}

int pokemon_size(t_pokemon* pokemon){
	return strlen(pokemon->especie) + posicion_size(pokemon->posicion) ;
}

void pokemon_destroy(t_pokemon* pokemon){
	free(pokemon->especie);
	free(pokemon);
}
