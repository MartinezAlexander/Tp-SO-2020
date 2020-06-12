#include "localized_pokemon.h"
#include <commons/string.h>

t_localized_pokemon* localized_pokemon_create(char* nombre, t_list* posiciones){
	t_localized_pokemon* localized_pokemon = malloc( sizeof(t_localized_pokemon) );
	localized_pokemon->nombre = string_from_format("%s",nombre);
	localized_pokemon->tamanio_nombre = strlen(nombre);
	localized_pokemon->posiciones = posiciones;
	localized_pokemon->cantidadPos = (uint32_t)list_size(posiciones);
	return localized_pokemon;
}

op_code localized_pokemon_codigo(t_localized_pokemon* localized_pokemon){
	return LOCALIZED_POKEMON;
}

t_buffer* localized_pokemon_to_buffer(t_localized_pokemon* localized_pokemon){
	t_buffer* buffer = malloc( sizeof(t_buffer) );
	//tam_nombre y cant posiciones, nombre, posiciones
	buffer->size = sizeof(uint32_t) * 2
			+ localized_pokemon->tamanio_nombre
			+ (sizeof(uint32_t) * 2) * localized_pokemon->cantidadPos;
	buffer->stream = localized_pokemon_to_stream(localized_pokemon);
	return buffer;
}

t_localized_pokemon* localized_pokemon_from_buffer(t_buffer* buffer){
	return localized_pokemon_from_stream(buffer->stream);
}

void localized_pokemon_mostrar(t_localized_pokemon* localized_pokemon){
	printf("Mensaje - Localized Pokemon:\n");
	printf("Nombre: %s\n",localized_pokemon->nombre);
	printf("Tamanio de nombre: %d\n",localized_pokemon->tamanio_nombre);
	printf("Cantidad de posiciones: %d\n",localized_pokemon->cantidadPos);
	printf("Posiciones: \n");
	for(int i=0; i<localized_pokemon->cantidadPos; i++){
		uint32_t x = posiciones_get_X(localized_pokemon->posiciones,i);
		uint32_t y = posiciones_get_Y(localized_pokemon->posiciones,i);
		printf("Posicion numero %d: x = %d , y = %d \n",i+1,x,y);
	}
	puts("------------");
}

char* localized_pokemon_to_string(t_localized_pokemon* localized_pokemon){

	char* mensaje = string_from_format("Tipo = LOCALIZED_POKEMON | Contenido = Pokemon: %s | Cantidad de posiciones: %d | Posiciones (x,y): ",localized_pokemon->nombre,localized_pokemon->cantidadPos);
	for(int i=0; i<localized_pokemon->cantidadPos; i++){
		uint32_t x = posiciones_get_X(localized_pokemon->posiciones,i);
		uint32_t y = posiciones_get_Y(localized_pokemon->posiciones,i);
		char* posicion = string_from_format("(%d,%d) ",x,y);
		string_append(&mensaje,posicion);
		free(posicion);
	}
	return mensaje;
}

void localized_pokemon_destroy(t_localized_pokemon* localized_pokemon){
	free(localized_pokemon->nombre);
	posiciones_destroy(localized_pokemon->posiciones);
	free(localized_pokemon);
}

int localized_pokemon_size(t_localized_pokemon* localized){
	return sizeof(localized->tamanio_nombre) + localized->tamanio_nombre + sizeof(localized->cantidadPos) + (sizeof(uint32_t)*2)*localized->cantidadPos;
}

t_list* localized_pokemon_get_list(t_localized_pokemon* localized_pokemon){
	t_list* pokemons = list_create();

	for(int i = 0 ; i < localized_pokemon->cantidadPos ; i++){
		t_pokemon* pokemon = pokemon_create(localized_pokemon->nombre,*((t_posicion*)list_get(localized_pokemon->posiciones, i)));
		list_add(pokemons, pokemon);
	}

	return pokemons;
}

void* localized_pokemon_to_stream(t_localized_pokemon* localized_pokemon){
	//tam_nombre y cant posiciones, nombre, posiciones
	int size = sizeof(uint32_t) * 2
			+ localized_pokemon->tamanio_nombre
			+ (sizeof(uint32_t) * 2) * localized_pokemon->cantidadPos;

	void* stream = malloc(size);
	int offset = 0;

	memcpy(stream + offset, &(localized_pokemon->tamanio_nombre), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, localized_pokemon->nombre, localized_pokemon->tamanio_nombre);
	offset += localized_pokemon->tamanio_nombre;
	memcpy(stream + offset, &(localized_pokemon->cantidadPos), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	//Por cada posicion segun la cantidad, hago memcpy de x e y.
	for(int i = 0 ; i < localized_pokemon->cantidadPos ; i++){
		uint32_t x = posiciones_get_X(localized_pokemon->posiciones,i);
		memcpy(stream + offset, &(x), sizeof(uint32_t));
		offset += sizeof(uint32_t);
		uint32_t y = posiciones_get_Y(localized_pokemon->posiciones,i);
		memcpy(stream + offset, &(y), sizeof(uint32_t));
		offset += sizeof(uint32_t);
	}

	return stream;
}

t_localized_pokemon* localized_pokemon_from_stream(void* stream){
	t_localized_pokemon* localized_pokemon = malloc( sizeof(t_localized_pokemon) );

	memcpy(&(localized_pokemon->tamanio_nombre), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	localized_pokemon->nombre = malloc(localized_pokemon->tamanio_nombre + 1);
	memcpy(localized_pokemon->nombre, stream, localized_pokemon->tamanio_nombre);
	localized_pokemon->nombre[localized_pokemon->tamanio_nombre + 1] = '\0';
	stream += localized_pokemon->tamanio_nombre;
	memcpy(&(localized_pokemon->cantidadPos), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	localized_pokemon->posiciones = posiciones_create();
	for(int i = 0 ; i < localized_pokemon->cantidadPos ; i++){
		t_posicion* posicion = malloc(sizeof(t_posicion));
		memcpy(&(posicion->posicionX), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);
		memcpy(&(posicion->posicionY), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);
		list_add(localized_pokemon->posiciones,posicion);
	}

	return localized_pokemon;
}
