#include "get_pokemon.h"
#include <commons/string.h>

t_get_pokemon* get_pokemon_create(char* nombre){
	t_get_pokemon* get_pokemon = malloc( sizeof(t_get_pokemon) );
	get_pokemon->nombre = nombre;
	get_pokemon->tamanio_nombre = strlen(nombre) + 1;
	return get_pokemon;
}

op_code get_pokemon_codigo(t_get_pokemon* get_pokemon){
	return GET_POKEMON;
}

t_buffer* get_pokemon_to_buffer(t_get_pokemon* get_pokemon){
	t_buffer* buffer = malloc( sizeof(t_buffer) );
	buffer->size = sizeof(uint32_t) + get_pokemon->tamanio_nombre;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &(get_pokemon->tamanio_nombre), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, get_pokemon->nombre, get_pokemon->tamanio_nombre);
	offset += get_pokemon->tamanio_nombre;

	buffer->stream = stream;

	return buffer;
}

t_get_pokemon* get_pokemon_from_buffer(t_buffer* buffer){
	t_get_pokemon* get_pokemon = malloc( sizeof(t_get_pokemon) );
	void* stream = buffer->stream;

	memcpy(&(get_pokemon->tamanio_nombre), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	get_pokemon->nombre = malloc(get_pokemon->tamanio_nombre);
	memcpy(get_pokemon->nombre, stream, get_pokemon->tamanio_nombre);
	stream += get_pokemon->tamanio_nombre;

	return get_pokemon;
}

void get_pokemon_mostrar(t_get_pokemon* get_pokemon){
	printf("Mensaje - Get Pokemon:\n");
	printf("Nombre: %s\n",get_pokemon->nombre);
	printf("Tamanio de nombre: %d\n",get_pokemon->tamanio_nombre);
	puts("------------");
}

char* get_pokemon_to_string(t_get_pokemon* get_pokemon){
	return string_from_format("Mensaje - Get Pokemon:\nNombre: %s\nTamanio de nombre: %d\n------------",get_pokemon->nombre,get_pokemon->tamanio_nombre);
}

void get_pokemon_destroy(t_get_pokemon* get_pokemon){
	free(get_pokemon);
}
