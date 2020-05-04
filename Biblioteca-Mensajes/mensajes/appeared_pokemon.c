#include "appeared_pokemon.h"
#include <commons/string.h>

t_appeared_pokemon* appeared_pokemon_create(char* nombre, uint32_t posX, uint32_t posY){
	t_appeared_pokemon* appeared_pokemon = malloc( sizeof(t_appeared_pokemon) );
	appeared_pokemon->posicion.posicionX = posX;
	appeared_pokemon->posicion.posicionY = posY;
	appeared_pokemon->nombre = nombre;
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
	memcpy(stream + offset, appeared_pokemon->nombre, appeared_pokemon->tamanio_nombre);
	offset += appeared_pokemon->tamanio_nombre;
	memcpy(stream + offset, &(appeared_pokemon->posicion.posicionX), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(appeared_pokemon->posicion.posicionY), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	return buffer;
}

t_appeared_pokemon* appeared_pokemon_from_buffer(t_buffer* buffer){
	t_appeared_pokemon* appeared_pokemon = malloc( sizeof(t_appeared_pokemon) );
	void* stream = buffer->stream;

	memcpy(&(appeared_pokemon->tamanio_nombre), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	appeared_pokemon->nombre = malloc(appeared_pokemon->tamanio_nombre);
	memcpy(appeared_pokemon->nombre, stream, appeared_pokemon->tamanio_nombre);
	stream += appeared_pokemon->tamanio_nombre;
	memcpy(&(appeared_pokemon->posicion.posicionX), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&(appeared_pokemon->posicion.posicionY), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return appeared_pokemon;
}

void appeared_pokemon_mostrar(t_appeared_pokemon* appeared_pokemon){
	printf("Mensaje - Appeared Pokemon:\n");
	printf("Nombre: %s\n",appeared_pokemon->nombre);
	printf("Tamanio del nombre: %d\n",appeared_pokemon->tamanio_nombre);
	printf("Posicion x: %d\n",appeared_pokemon->posicion.posicionX);
	printf("Posicion y: %d\n",appeared_pokemon->posicion.posicionY);
	puts("------------");
}

char* appeared_pokemon_to_string(t_appeared_pokemon* appeared_pokemon){
	return string_from_format("Mensaje - Appeared Pokemon:\nNombre: %s\nTamanio del nombre: %d\nPosicion x: %d\nPosicion y: %d\n------------",appeared_pokemon->nombre,appeared_pokemon->tamanio_nombre,appeared_pokemon->posicion.posicionX,appeared_pokemon->posicion.posicionY);
}

void appeared_pokemon_destroy(t_appeared_pokemon* appeared_pokemon){
	free(appeared_pokemon);
}
