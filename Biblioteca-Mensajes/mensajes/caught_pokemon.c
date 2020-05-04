#include "caught_pokemon.h"
#include <commons/string.h>

t_caught_pokemon* caught_pokemon_create(uint32_t fueAtrapado){
	t_caught_pokemon* caught_pokemon = malloc( sizeof(t_caught_pokemon) );
	caught_pokemon->atrapado = fueAtrapado;
	return caught_pokemon;
}

op_code caught_pokemon_codigo(t_caught_pokemon* caught_pokemon){
	return CAUGHT_POKEMON;
}

t_buffer* caught_pokemon_to_buffer(t_caught_pokemon* caught_pokemon){
	t_buffer* buffer = malloc( sizeof(t_buffer) );
	buffer->size = sizeof(uint32_t);

	void* stream = malloc(buffer->size);
	int offset = 0;
	memcpy(stream + offset, &(caught_pokemon->atrapado), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	return buffer;
}

t_caught_pokemon* caught_pokemon_from_buffer(t_buffer* buffer){
	t_caught_pokemon* caught_pokemon = malloc( sizeof(t_caught_pokemon) );
	void* stream = buffer->stream;

	memcpy(&(caught_pokemon->atrapado), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return caught_pokemon;
}

void caught_pokemon_mostrar(t_caught_pokemon* caught_pokemon){
	printf("Mensaje - Caught Pokemon:\n");
	printf("Fue atrapado: %d\n",caught_pokemon->atrapado);
	puts("------------");
}

char* caught_pokemon_to_string(t_caught_pokemon* caught_pokemon){
	return string_from_format("Mensaje - Caught Pokemon:\nFue atrapado: %d\n------------",caught_pokemon->atrapado);
}

void caught_pokemon_destroy(t_caught_pokemon* caught_pokemon){
	free(caught_pokemon);
}
