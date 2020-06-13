#include "new_pokemon.h"
#include <commons/string.h>

t_new_pokemon* new_pokemon_create(char* nombre, uint32_t posX, uint32_t posY, uint32_t cantidad){
	t_new_pokemon* new_pokemon = malloc( sizeof(t_new_pokemon) );
	new_pokemon->pokemon = pokemon_create(nombre,posicion_create(posX,posY));
	new_pokemon->cantidad = cantidad;
	new_pokemon->tamanio_nombre = strlen(nombre);
    return new_pokemon;
}

op_code new_pokemon_codigo(t_new_pokemon* new_pokemon){
	return NEW_POKEMON;
}

t_buffer* new_pokemon_to_buffer(t_new_pokemon* new_pokemon){
	t_buffer* buffer = malloc( sizeof(t_buffer) );
	buffer->size = sizeof(uint32_t)*4 + new_pokemon->tamanio_nombre;
	buffer->stream = new_pokemon_to_stream(new_pokemon);
	return buffer;
}

t_new_pokemon* new_pokemon_from_buffer(t_buffer* buffer){
	return new_pokemon_from_stream(buffer->stream);
}

void new_pokemon_mostrar(t_new_pokemon* new_pokemon){
	printf("Mensaje - New Pokemon:\n");
	printf("Nombre: %s\n",new_pokemon->pokemon->especie);
	printf("Tamanio del nombre: %d\n",new_pokemon->tamanio_nombre);
	printf("Posicion x: %d\n",new_pokemon->pokemon->posicion.posicionX);
	printf("Posicion y: %d\n",new_pokemon->pokemon->posicion.posicionY);
	printf("Cantidad: %d\n",new_pokemon->cantidad);
	puts("------------");
}

char* new_pokemon_to_string(t_new_pokemon* new_pokemon){
	return string_from_format("Tipo = NEW_POKEMON | Contenido = Pokemon: %s | Posicion (x,y):(%d,%d) | Cantidad: %d ",new_pokemon->pokemon->especie,new_pokemon->pokemon->posicion.posicionX,new_pokemon->pokemon->posicion.posicionY,new_pokemon->cantidad);
}

int new_pokemon_size(t_new_pokemon* new){
	return sizeof(new->cantidad) + sizeof(new->tamanio_nombre) + pokemon_size(new->pokemon);
}

void new_pokemon_destroy(t_new_pokemon* new_pokemon){
	pokemon_destroy(new_pokemon->pokemon);
	free(new_pokemon);
}

void* new_pokemon_to_stream(t_new_pokemon* new_pokemon){
	int size = sizeof(uint32_t)*4 + new_pokemon->tamanio_nombre;

	void* stream = malloc(size);
	int offset = 0; // Desplazamiento

	memcpy(stream + offset, &(new_pokemon->tamanio_nombre), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, new_pokemon->pokemon->especie, new_pokemon->tamanio_nombre);
	offset += new_pokemon->tamanio_nombre;
	memcpy(stream + offset, &(new_pokemon->pokemon->posicion.posicionX), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(new_pokemon->pokemon->posicion.posicionY), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(new_pokemon->cantidad), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	return stream;
}

t_new_pokemon* new_pokemon_from_stream(void* stream){
	t_new_pokemon* new_pokemon = malloc( sizeof(t_new_pokemon));
	new_pokemon->pokemon = malloc(sizeof(t_pokemon));

	memcpy(&(new_pokemon->tamanio_nombre), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	new_pokemon->pokemon->especie = malloc(new_pokemon->tamanio_nombre + 1);
	memcpy(new_pokemon->pokemon->especie,stream, new_pokemon->tamanio_nombre);
	new_pokemon->pokemon->especie[new_pokemon->tamanio_nombre] = '\0';
	stream += (new_pokemon->tamanio_nombre);
	memcpy(&(new_pokemon->pokemon->posicion.posicionX), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&(new_pokemon->pokemon->posicion.posicionY), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&(new_pokemon->cantidad), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return new_pokemon;
}
