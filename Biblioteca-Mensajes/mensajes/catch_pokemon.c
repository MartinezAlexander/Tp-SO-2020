#include "catch_pokemon.h"
#include <commons/string.h>

t_catch_pokemon* catch_pokemon_create(char* nombre, uint32_t posX, uint32_t posY){
	t_catch_pokemon* catch_pokemon = malloc( sizeof(t_catch_pokemon) );
	catch_pokemon->pokemon = pokemon_create(nombre,posicion_create(posX,posY));
	catch_pokemon->tamanio_nombre = strlen(nombre) + 1;
	return catch_pokemon;
}

op_code catch_pokemon_codigo(t_catch_pokemon* catch_pokemon){
	return CATCH_POKEMON;
}

t_buffer* catch_pokemon_to_buffer(t_catch_pokemon* catch_pokemon){
	t_buffer* buffer = malloc( sizeof(t_buffer) );
	buffer->size = sizeof(uint32_t) * 3 + catch_pokemon->tamanio_nombre;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &(catch_pokemon->tamanio_nombre), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, catch_pokemon->pokemon->especie, catch_pokemon->tamanio_nombre);
	offset += catch_pokemon->tamanio_nombre;
	memcpy(stream + offset, &(catch_pokemon->pokemon->posicion.posicionX), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(catch_pokemon->pokemon->posicion.posicionY), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	return buffer;
}

t_catch_pokemon* catch_pokemon_from_buffer(t_buffer* buffer){
	t_catch_pokemon* catch_pokemon = malloc( sizeof(t_catch_pokemon) );
	void* stream = buffer->stream;
	catch_pokemon->pokemon = malloc( sizeof(t_pokemon) );

	memcpy(&(catch_pokemon->tamanio_nombre), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	catch_pokemon->pokemon->especie = malloc(catch_pokemon->tamanio_nombre);
	memcpy(catch_pokemon->pokemon->especie, stream, catch_pokemon->tamanio_nombre);
	stream += catch_pokemon->tamanio_nombre;
	memcpy(&(catch_pokemon->pokemon->posicion.posicionX), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&(catch_pokemon->pokemon->posicion.posicionY), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return catch_pokemon;
}
void catch_pokemon_mostrar(t_catch_pokemon* catch_pokemon){
	printf("Mensaje - Catch Pokemon:\n");
	printf("Nombre: %s\n",catch_pokemon->pokemon->especie);
	printf("Tamanio del nombre: %d\n",catch_pokemon->tamanio_nombre);
	printf("Posicion x: %d\n",catch_pokemon->pokemon->posicion.posicionX);
	printf("Posicion y: %d\n",catch_pokemon->pokemon->posicion.posicionY);
	puts("------------");
}

char* catch_pokemon_to_string(t_catch_pokemon*catch_pokemon){
	return string_from_format("Tipo = CATCH_POKEMON | Contenido = Pokemon: %s | Posicion (x,y): (%d,%d) ",catch_pokemon->pokemon->especie,catch_pokemon->pokemon->posicion.posicionX,catch_pokemon->pokemon->posicion.posicionY);
}

int catch_pokemon_size(t_catch_pokemon* pokemon){
	return sizeof(pokemon->tamanio_nombre) + pokemon_size(pokemon->pokemon);
}

void catch_pokemon_destroy(t_catch_pokemon* catch_pokemon){
	pokemon_destroy(catch_pokemon->pokemon);
	free(catch_pokemon);
}

