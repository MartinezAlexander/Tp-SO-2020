#include "localized_pokemon.h"
#include <commons/string.h>

t_localized_pokemon* localized_pokemon_create(char* nombre,uint32_t cantidadPosiciones, t_posicion* posiciones){
	t_localized_pokemon* localized_pokemon = malloc( sizeof(t_localized_pokemon) );
	localized_pokemon->nombre = nombre;
	localized_pokemon->tamanio_nombre = strlen(nombre) + 1;
	localized_pokemon->cantidadPos = cantidadPosiciones;
	localized_pokemon->posiciones = posiciones;
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

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &(localized_pokemon->tamanio_nombre), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, localized_pokemon->nombre, localized_pokemon->tamanio_nombre);
	offset += localized_pokemon->tamanio_nombre;
	memcpy(stream + offset, &(localized_pokemon->cantidadPos), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	//Por cada posicion segun la cantidad, hago memcpy de x e y.
	for(int i = 0 ; i < localized_pokemon->cantidadPos ; i++){
		memcpy(stream + offset, &(localized_pokemon->posiciones[i].posicionX), sizeof(uint32_t));
		offset += sizeof(uint32_t);
		memcpy(stream + offset, &(localized_pokemon->posiciones[i].posicionY), sizeof(uint32_t));
		offset += sizeof(uint32_t);
	}

	buffer->stream = stream;

	return buffer;
}

t_localized_pokemon* localized_pokemon_from_buffer(t_buffer* buffer){
	t_localized_pokemon* localized_pokemon = malloc( sizeof(t_localized_pokemon) );

	void* stream = buffer->stream;

	memcpy(&(localized_pokemon->tamanio_nombre), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	localized_pokemon->nombre = malloc(localized_pokemon->tamanio_nombre);
	memcpy(localized_pokemon->nombre, stream, localized_pokemon->tamanio_nombre);
	stream += localized_pokemon->tamanio_nombre;
	memcpy(&(localized_pokemon->cantidadPos), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	localized_pokemon->posiciones = malloc(sizeof(t_posicion) * localized_pokemon->cantidadPos);
	for(int i = 0 ; i < localized_pokemon->cantidadPos ; i++){
		memcpy(&(localized_pokemon->posiciones[i].posicionX), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);
		memcpy(&(localized_pokemon->posiciones[i].posicionY), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);
	}

	return localized_pokemon;
}

void localized_pokemon_mostrar(t_localized_pokemon* localized_pokemon){
	printf("Mensaje - Localized Pokemon:\n");
	printf("Nombre: %s\n",localized_pokemon->nombre);
	printf("Tamanio de nombre: %d\n",localized_pokemon->tamanio_nombre);
	printf("Cantidad de posiciones: %d\n",localized_pokemon->cantidadPos);
	printf("Posiciones: \n");
	for(int i=0; i<localized_pokemon->cantidadPos; i++){
		printf("Posicion numero %d: x = %d , y = %d \n",i+1,localized_pokemon->posiciones[i].posicionX,localized_pokemon->posiciones[i].posicionY);
	}
	puts("------------");
}

char* localized_pokemon_to_string(t_localized_pokemon* localized_pokemon){
	char* mensaje = string_from_format("Mensaje - Localized Pokemon:\nNombre: %s\nTamanio de nombre: %d\nCantidad de posiciones: %d\nPosiciones: \n",localized_pokemon->nombre,localized_pokemon->tamanio_nombre,localized_pokemon->cantidadPos);
	for(int i=0; i<localized_pokemon->cantidadPos; i++){
			char* posicion = string_from_format("Posicion numero %d: x = %d , y = %d \n",i+1,localized_pokemon->posiciones[i].posicionX,localized_pokemon->posiciones[i].posicionY);
			string_append(&mensaje,posicion);
	}
	string_append(&mensaje,"------------");
	return mensaje;
}

void localized_pokemon_destroy(t_localized_pokemon* localized_pokemon){
	free(localized_pokemon->posiciones);
	free(localized_pokemon);
}
