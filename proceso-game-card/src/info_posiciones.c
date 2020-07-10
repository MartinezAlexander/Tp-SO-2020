/*
 * info_posiciones.c
 *
 *  Created on: 9 jul. 2020
 *      Author: utnso
 */


#include "info_posiciones.h"

char* info_pokemon_to_string(t_posicion posicion, int cantidad){
	return string_from_format("%d-%d=%d",posicion.posicionX,posicion.posicionY,cantidad);
}

info_posicion* info_posicion_from_string(char* posicion){
	info_posicion* posicion_real = malloc(sizeof(info_posicion));
	char** posicion_aux = string_split(posicion,"-");
	posicion_real->posicion.posicionX = atoi(posicion_aux[0]);
	char** posicion_aux2 = string_split(posicion_aux[1],"=");
	posicion_real->posicion.posicionY = atoi(posicion_aux2[0]);
	posicion_real->cantidad = atoi(posicion_aux2[1]);
	return posicion_real;
}

t_list* convertir_info_posiciones(char* posiciones_string){
	char** posiciones = string_split(posiciones_string,"\n");
	int i = 0;

	t_list* posiciones_reales = list_create();

	while(posiciones[i] != NULL){
		info_posicion* posicion_real = info_posicion_from_string(posiciones[i]);
		list_add(posiciones_reales,posicion_real);
		i++;
	}

	return posiciones_reales;
}

void info_posicion_mostrar(info_posicion* posicion){
	printf("x=%d, y=%d, cantidad=%d\n",posicion->posicion.posicionX,posicion->posicion.posicionY,posicion->cantidad);
}

char* lista_info_posicion_to_string(t_list* posiciones){
	char* string = string_new();

	for(int i=0; i<list_size(posiciones); i++){
		info_posicion* posicion = list_get(posiciones,i);
		char* posicion_string = info_pokemon_to_string(posicion->posicion,posicion->cantidad);
		string_append(&string,posicion_string);
		string_append(&string,"\n");
	}

	return string;
}

