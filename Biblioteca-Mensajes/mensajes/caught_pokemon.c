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
	buffer->stream = caught_pokemon_to_stream(caught_pokemon);
	return buffer;
}

t_caught_pokemon* caught_pokemon_from_buffer(t_buffer* buffer){
	return caught_pokemon_from_stream(buffer->stream);
}

void caught_pokemon_mostrar(t_caught_pokemon* caught_pokemon){
	printf("Mensaje - Caught Pokemon:\n");
	printf("Fue atrapado: %d\n",caught_pokemon->atrapado);
	puts("------------");
}

char* caught_pokemon_to_string(t_caught_pokemon* caught_pokemon){
	char* atrapado = "Atrapado";
	char* no_atrapado = "No atrapado";
	char* resultado;
	if(caught_pokemon->atrapado == 0){
		resultado = no_atrapado;
	}else{
		resultado = atrapado;
	}
	return string_from_format("Tipo = CAUGHT_POKEMON | Contenido = %s ",resultado);
}

int caught_pokemon_size(t_caught_pokemon* caught){
	return sizeof(caught->atrapado);
}

void caught_pokemon_destroy(t_caught_pokemon* caught_pokemon){
	free(caught_pokemon);
}

void* caught_pokemon_to_stream(t_caught_pokemon* caught_pokemon){
	int size = sizeof(uint32_t);

	void* stream = malloc(size);
	int offset = 0;
	memcpy(stream + offset, &(caught_pokemon->atrapado), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	return stream;
}

t_caught_pokemon* caught_pokemon_from_stream(void* stream){
	t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));

	memcpy(&(caught_pokemon->atrapado), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return caught_pokemon;
}
