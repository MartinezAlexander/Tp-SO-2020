/*
 * info_posiciones.c
 *
 *  Created on: 9 jul. 2020
 *      Author: utnso
 */


#include "info_posiciones.h"

/*
 * Dado un listado de info_posicion, devuelve el tamanio que ocuparia en disco
 */
int tamanio_info_posiciones(t_list* renglones){
	char* renglones_string = lista_info_posicion_to_string(renglones);
	int length = string_length(renglones_string);
	free(renglones_string);
	return length;
}

/*
 * Dada una posicion, busca la posicion en la lista de info_posicion, y de encontrarla
 * agrega la cantidad dada.
 * En caso de que no exista la posicion, agrega un nuevo item a la lista
 */
void agregar_info_posicion_a_listado(t_list* renglones, t_posicion posicion, int cantidad){
	int no_encontre = 1;

	for(int i = 0; i<list_size(renglones);i++){
		info_posicion* posicion_info = list_get(renglones,i);
		if(posicion_info->posicion.posicionX == posicion.posicionX && posicion_info->posicion.posicionY == posicion.posicionY){
			no_encontre = 0;
			posicion_info->cantidad += cantidad;
		}
	}

	if(no_encontre){
		info_posicion* nueva_info = malloc(sizeof(info_posicion));
		nueva_info->posicion = posicion;
		nueva_info->cantidad = cantidad;
		list_add(renglones,nueva_info);
	}
}

/*
 * Retorna 1 si decremento la posicion y 0 en caso de que no exista
 */
int decrementar_info_posicion_en_listado(t_list* renglones, t_posicion posicion){

	for(int i=0; i<list_size(renglones); i++){
		info_posicion* posicion_info = list_get(renglones,i);
		if(posicion_info->posicion.posicionX == posicion.posicionX && posicion_info->posicion.posicionY == posicion.posicionY){

			posicion_info->cantidad--;

			if(posicion_info->cantidad == 0){
				list_remove(renglones,i);
			}

			return 1;
		}
	}

	return 0;
}

char* info_pokemon_to_string(t_posicion posicion, int cantidad){
	return string_from_format("%d-%d=%d",posicion.posicionX,posicion.posicionY,cantidad);
}

void freeargpointer(char** array)
{
    int i;
    for ( i = 0; array[i]; i++ )
        free( array[i] );
    free( array );
}

info_posicion* info_posicion_from_string(char* posicion){

	info_posicion* posicion_real = malloc(sizeof(info_posicion));

	char** posicion_aux = string_split(posicion,"-");
	posicion_real->posicion.posicionX = atoi(posicion_aux[0]);

	char** posicion_aux2 = string_split(posicion_aux[1],"=");
	posicion_real->posicion.posicionY = atoi(posicion_aux2[0]);
	posicion_real->cantidad = atoi(posicion_aux2[1]);

	/*puts("Free 1");
	freeargpointer(posicion_aux);
	puts("Free 2");
	freeargpointer(posicion_aux2);*/

	return posicion_real;
}

t_list* convertir_info_posiciones(char* posiciones_string){
	t_list* posiciones_reales = list_create();

	if(string_length(posiciones_string) > 0){

		char** posiciones = string_split(posiciones_string, "\n");
		int i = 0;

		while (posiciones[i] != NULL) {
			info_posicion* posicion_real = info_posicion_from_string(posiciones[i]);
			list_add(posiciones_reales, posicion_real);
			i++;
		}
		//TODO liberar array de posiciones_string
		free(posiciones);
		free(posiciones_string);

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

