#ifndef POSICIONES_H
#define POSICIONES_H

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct{
	uint32_t posicionX;
	uint32_t posicionY;
}t_posicion;

/*
posicion_create es un metodo que crea una posicion sin necesidad
de preocuparse como esta implementada dicha estructura solo indicando
una valor para el x y otro para el y.
ej: t_posicion posicion_nueva = posicion_create(2,3);
*/
t_posicion posicion_create(uint32_t posX, uint32_t posY);

/*
posiciones_create te crea una lista de posiciones de la cantidad que le indiques
ej: t_posicion* posiciones = posiciones_create(3);
te crea una lista de 3 posiciones
*/
t_list* posiciones_create();

/*
una forma de agrega posiciones a la lista de posiciones es indicando el valor de
x e y que tenga cada posicion que se desee agregar por lo cual este metodo facilita
esto posiciones_add_XY ya que necesita la lista de posiciones y los valores de (x,y).
ej: t_posicion* posiciones = posiciones_create(3);
posiciones_add_XY(posiciones, 4, 5);
ahi agregue a la lista la posicion (x = 4, y = 5);
*/
void posiciones_add_XY(t_list* posiciones, t_posicion* posicion);

/*
Dada una lista de posiciones y un indice, me devuelve la 'x' de la posicion en
dicho indice.
Ej: uint32_t  = posiciones_get_X(posiciones, 4)  devuelve la x de la cuarta posicion
de la lista.
 */
uint32_t posiciones_get_X(t_list* posiciones, int indice);
uint32_t posiciones_get_Y(t_list* posiciones, int indice);

/*
posiciones_destroy libera la memoria que se utilizo para crear la lista de posiciones
*/
void posiciones_destroy(t_list* posiciones);

/*
posiciones_destroy_elements deja vacia un lista de posiciones liberando sus elementos
*/
void posiciones_destroy_elements(t_list* posiciones);

/*
Dada una posicion te devuelve cuantos bytes ocupa
*/
int posicion_size(t_posicion posicion);

#endif
