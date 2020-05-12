#ifndef POSICIONES_H
#define POSICIONES_H

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct{
	uint32_t posicionX;
	uint32_t posicionY;
}t_posicion;
/*
typedef struct{
	t_posicion* posiciones;
	int cantidad;
	int indice;
}t_posiciones;
*/
/*
posicion_create es un metodo que crea una posicion sin necesidad
de preocuparse como esta implementada dicha estructura solo indicando
una valor para el x y otro para el y.
ej: t_posicion posicion_nueva = posicion_create(2,3);
*/
t_posicion* posicion_create(uint32_t posX, uint32_t posY);

/*
posicion_get_X te retorna el valor de la coordenada X de una posicion determinada
ej: t_posicion posicion_nueva = posicion_create(4,5);
uint32_t x = posicion_get_X(posicion_nueva);
con esto guardamos en x el valor 4
*/
uint32_t* posicion_get_X(t_posicion* posicion);
uint32_t* posicion_get_Y(t_posicion* posicion);

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
void posiciones_add_XY(t_list* posiciones, uint32_t posX, uint32_t posY);

/*
Dada una lista de posiciones y un indice, me devuelve la 'x' de la posicion en
dicho indice.
Ej: uint32_t  = posiciones_get_X(posiciones, 4)  devuelve la x de la cuarta posicion
de la lista.
 */
uint32_t* posiciones_get_X(t_list* posiciones, int indice);
uint32_t* posiciones_get_Y(t_list* posiciones, int indice);

/*
posiciones_destroy libera la memoria que se utilizo para crear la lista de posiciones
*/
void posiciones_destroy(t_list* posiciones);

#endif