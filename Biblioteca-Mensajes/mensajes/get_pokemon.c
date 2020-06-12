#include "get_pokemon.h"
#include <commons/string.h>

t_get_pokemon* get_pokemon_create(char* nombre){
	t_get_pokemon* get_pokemon = malloc( sizeof(t_get_pokemon) );
	get_pokemon->nombre = string_from_format("%s",nombre);
	get_pokemon->tamanio_nombre = strlen(nombre);
	return get_pokemon;
}

op_code get_pokemon_codigo(t_get_pokemon* get_pokemon){
	return GET_POKEMON;
}

t_buffer* get_pokemon_to_buffer(t_get_pokemon* get_pokemon){
	t_buffer* buffer = malloc( sizeof(t_buffer) );
	buffer->size = sizeof(uint32_t) + get_pokemon->tamanio_nombre;
	buffer->stream = get_pokemon_to_stream(get_pokemon);
	return buffer;
}

t_get_pokemon* get_pokemon_from_buffer(t_buffer* buffer){
	return get_pokemon_from_stream(buffer->stream);
}

void get_pokemon_mostrar(t_get_pokemon* get_pokemon){
	printf("Mensaje - Get Pokemon:\n");
	printf("Nombre: %s\n",get_pokemon->nombre);
	printf("Tamanio de nombre: %d\n",get_pokemon->tamanio_nombre);
	puts("------------");
}

char* get_pokemon_to_string(t_get_pokemon* get_pokemon){
	return string_from_format("Tipo = GET_POKEMON | Contenido = Pokemon: %s ",get_pokemon->nombre);
}

int get_pokemon_size(t_get_pokemon* get){
	return sizeof(get->tamanio_nombre) + get->tamanio_nombre;
}

void get_pokemon_destroy(t_get_pokemon* get_pokemon){
	free(get_pokemon->nombre);
	free(get_pokemon);
}

void* get_pokemon_to_stream(t_get_pokemon* get_pokemon){
	int size = sizeof(uint32_t) + get_pokemon->tamanio_nombre;

	void* stream = malloc(size);
	int offset = 0;

	memcpy(stream + offset, &(get_pokemon->tamanio_nombre), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, get_pokemon->nombre, get_pokemon->tamanio_nombre);
	offset += get_pokemon->tamanio_nombre;

	return stream;
}

t_get_pokemon* get_pokemon_from_stream(void* stream){
	t_get_pokemon* get_pokemon = malloc( sizeof(t_get_pokemon) );

	memcpy(&(get_pokemon->tamanio_nombre), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	get_pokemon->nombre = malloc(get_pokemon->tamanio_nombre + 1);
	memcpy(get_pokemon->nombre, stream, get_pokemon->tamanio_nombre);
	get_pokemon->nombre[get_pokemon->tamanio_nombre] = '\0';
	stream += get_pokemon->tamanio_nombre;

	return get_pokemon;
}
