/*
 * info_posiciones.h
 *
 *  Created on: 9 jul. 2020
 *      Author: utnso
 */

#ifndef INFO_POSICIONES_H_
#define INFO_POSICIONES_H_

#include <mensajes/posiciones.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct{
	t_posicion posicion;
	int cantidad;
}info_posicion;

char* info_pokemon_to_string(t_posicion posicion, int cantidad);
t_list* convertir_info_posiciones(char* posiciones);
void info_posicion_mostrar(info_posicion* posicion);
char* lista_info_posicion_to_string(t_list* posiciones);

int tamanio_info_posiciones(t_list* renglones);
void agregar_info_posicion_a_listado(t_list* renglones, t_posicion posicion, int cantidad);
int decrementar_info_posicion_en_listado(t_list* renglones, t_posicion posicion);

#endif /* INFO_POSICIONES_H_ */
