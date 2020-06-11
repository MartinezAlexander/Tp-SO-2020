#include "appeared_pokemon.h"
#include <commons/string.h>

t_appeared_pokemon* appeared_pokemon_create(char* nombre, uint32_t posX, uint32_t posY){
	t_appeared_pokemon* appeared_pokemon = malloc( sizeof(t_appeared_pokemon) );
	appeared_pokemon->pokemon = pokemon_create(nombre,posicion_create(posX,posY));
	appeared_pokemon->tamanio_nombre = strlen(nombre) + 1;
    return appeared_pokemon;
}

op_code appeared_pokemon_codigo(t_appeared_pokemon* appeared_pokemon){
	return APPEARED_POKEMON;
}

t_buffer* appeared_pokemon_to_buffer(t_appeared_pokemon* appeared_pokemon){
	t_buffer* buffer = malloc( sizeof(t_buffer) );
	buffer->size = sizeof(uint32_t) * 3 + appeared_pokemon->tamanio_nombre;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &(appeared_pokemon->tamanio_nombre), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, appeared_pokemon->pokemon->especie, appeared_pokemon->tamanio_nombre);
	offset += appeared_pokemon->tamanio_nombre;
	memcpy(stream + offset, &(appeared_pokemon->pokemon->posicion.posicionX), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(appeared_pokemon->pokemon->posicion.posicionY), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	return buffer;
}

t_appeared_pokemon* appeared_pokemon_from_buffer(t_buffer* buffer){
	t_appeared_pokemon* appeared_pokemon = malloc( sizeof(t_appeared_pokemon) );
	void* stream = buffer->stream;

	appeared_pokemon->pokemon = malloc( sizeof(t_pokemon) );

	memcpy(&(appeared_pokemon->tamanio_nombre), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	appeared_pokemon->pokemon->especie = malloc(appeared_pokemon->tamanio_nombre);
	memcpy(appeared_pokemon->pokemon->especie, stream, appeared_pokemon->tamanio_nombre);
	stream += appeared_pokemon->tamanio_nombre;
	memcpy(&(appeared_pokemon->pokemon->posicion.posicionX), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&(appeared_pokemon->pokemon->posicion.posicionY), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return appeared_pokemon;
}

void appeared_pokemon_mostrar(t_appeared_pokemon* appeared_pokemon){
	printf("Mensaje - Appeared Pokemon:\n");
	printf("Nombre: %s\n",appeared_pokemon->pokemon->especie);
	printf("Tamanio del nombre: %d\n",appeared_pokemon->tamanio_nombre);
	printf("Posicion x: %d\n",appeared_pokemon->pokemon->posicion.posicionX);
	printf("Posicion y: %d\n",appeared_pokemon->pokemon->posicion.posicionY);
	puts("------------");
}

char* appeared_pokemon_to_string(t_appeared_pokemon* appeared_pokemon){
	return string_from_format("Tipo = APPEARED_POKEMON | Contenido = Pokemon: %s | Posicion (x,y): (%d,%d) ",appeared_pokemon->pokemon->especie,appeared_pokemon->pokemon->posicion.posicionX,appeared_pokemon->pokemon->posicion.posicionY);
}

int appeared_pokemon_size(t_appeared_pokemon* appeared){
	return sizeof(appeared->tamanio_nombre) + pokemon_size(appeared->pokemon);
}

void appeared_pokemon_destroy(t_appeared_pokemon* appeared_pokemon){
	pokemon_destroy(appeared_pokemon->pokemon);
	free(appeared_pokemon);
}
