#include "posiciones.h"
#include <stdlib.h>

t_posicion* posicion_create(uint32_t posX, uint32_t posY){
	t_posicion* posicion = malloc(sizeof(t_posicion));
	posicion->posicionX = posX;
	posicion->posicionY = posY;
	return posicion;
}

uint32_t* posicion_get_X(t_posicion* posicion){
	return &(posicion->posicionX);
}
uint32_t* posicion_get_Y(t_posicion* posicion){
	return &(posicion->posicionY);
}

t_list* posiciones_create(){
	return list_create();
}

void posiciones_add_XY(t_list* posiciones, uint32_t posX, uint32_t posY){
	list_add(posiciones,posicion_create(posX,posY));
}

uint32_t* posiciones_get_X(t_list* posiciones, int indice){
	t_posicion* posicion = list_get(posiciones,indice);
	return posicion_get_X(posicion);
}

uint32_t* posiciones_get_Y(t_list* posiciones, int indice){
	t_posicion* posicion = list_get(posiciones,indice);
	return posicion_get_Y(posicion);
}

int posicion_size(t_posicion posicion){
	return sizeof(posicion.posicionX) + sizeof(posicion.posicionY);
}

void posiciones_destroy(t_list* posiciones){
	list_destroy_and_destroy_elements(posiciones,free);
}