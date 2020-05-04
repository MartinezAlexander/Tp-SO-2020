#include "posiciones.h"
#include <stdlib.h>

t_posicion posicion_create(uint32_t posX, uint32_t posY){
	t_posicion posicion;
	posicion.posicionX = posX;
	posicion.posicionY = posY;
	return posicion;
}

uint32_t posicion_get_X(t_posicion posicion){
	return posicion.posicionX;
}
uint32_t posicion_get_Y(t_posicion posicion){
	return posicion.posicionY;
}

t_posiciones* posiciones_create(int cantidad){
	t_posiciones* posiciones = malloc( sizeof(t_posiciones) );
	posiciones->posiciones = malloc( sizeof(t_posicion) * cantidad );
	posiciones->cantidad = cantidad;
	posiciones->indice = 0;
	return posiciones;
}
void posiciones_add_XY(t_posiciones* posiciones, uint32_t posX, uint32_t posY){
	if(posiciones->indice < posiciones->cantidad){
		posiciones->posiciones[posiciones->indice].posicionX = posX;
		posiciones->posiciones[posiciones->indice].posicionY = posY;
		posiciones->indice++;
	}
}

void posiciones_add_posicion(t_posiciones* posiciones, t_posicion posicion){
	if (posiciones->indice < posiciones->cantidad) {
		posiciones->posiciones[posiciones->indice] = posicion;
		posiciones->indice++;
	}
}

t_posicion* posiciones_get_posicion_in(t_posiciones* posiciones, int indice){
	t_posicion* posicion = NULL;
	if(indice < posiciones->cantidad && indice >= 0 ){
		posicion = &posiciones->posiciones[indice];
	}
	return posicion;
}

void posiciones_destroy(t_posiciones* posiciones){
	free(posiciones);
}

/*
 * Ejemplo de uso

// forma de crear posiciones

t_posicion posicion_uno = posicion_create(2,3);  creo una posicion solo indicando x y
t_posicion posicion_dos = posicion_create(4,5);

// forma de crear una lista de posiciones

t_posiciones* posiciones = posiciones_create(3); creo una lista de posiciones de 3 elementos

// formas de agregar posiciones en una lista de posiciones

posiciones_add_posicion(posiciones,posicion_uno); agrego la posicion uno creada arriba
posiciones_add_posicion(posiciones,posicion_dos); agrego la posicion dos creada arriba
posiciones_add_XY(posiciones, 9, 9);   agrego una posicion sin tener que crear un t_posicion

for(int i=0; i<3; i++){

	t_posicion pos = (*posiciones_get_posicion_in(posiciones,i)); obtengo una posicion indicando el indice del array

	uint32_t x = posicion_get_X(pos); obtengo el valor x de una posicion
	uint32_t y = posicion_get_Y(pos); obtengo el valor y de una posicion

	printf("(x,y) = (%d,%d)\n",x,y);

}
 *
 * */

