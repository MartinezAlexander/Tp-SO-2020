#include "posiciones.h"
#include <stdlib.h>

t_posicion posicion_create(uint32_t posX, uint32_t posY){
	t_posicion posicion;
	posicion.posicionX = posX;
	posicion.posicionY = posY;
	return posicion;
}

t_list* posiciones_create(){
	return list_create();
}

void posiciones_add_XY(t_list* posiciones, t_posicion* posicion){
	list_add(posiciones,(void*)posicion);
}

uint32_t posiciones_get_X(t_list* posiciones, int indice){
	t_posicion* posicion = list_get(posiciones,indice);
	return posicion->posicionX;
}

uint32_t posiciones_get_Y(t_list* posiciones, int indice){
	t_posicion* posicion = list_get(posiciones,indice);
	return posicion->posicionY;
}

int posicion_size(t_posicion posicion){
	return sizeof(posicion.posicionX) + sizeof(posicion.posicionY);
}

void posicion_destroy(t_posicion* posicion){
	free(posicion);
}

void posiciones_destroy(t_list* posiciones){
	list_destroy(posiciones);
}

void posiciones_destroy_elements(t_list* posiciones){
	for(int i = 0; i< list_size(posiciones); i++){
		list_remove_and_destroy_element(posiciones,i,free);
	}
}
