/*
 * bitmap.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */
#include "bitmap.h"

void ocupar_bloque(int nuevo_bloque){
	bitarray_set_bit(estado_bloques->bitarray, nuevo_bloque);
}

int obtener_bloque_disponible(){
	for(int i = 0 ; i < blocks ; i++){
		if(!bitarray_test_bit(estado_bloques->bitarray, i)) return i;
	}
	printf("No se encontraron bloques disponibles");
	return -1;
	//TODO Si nos dicen que hacer en este caso en el enunciado resolver error.
}
