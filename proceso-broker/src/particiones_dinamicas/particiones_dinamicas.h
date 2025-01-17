
#ifndef MEMORIA_PARTICIONES_DINAMICAS_H_
#define MEMORIA_PARTICIONES_DINAMICAS_H_

#include <stdlib.h>
#include <time.h>
#include <mensajes/mensajes.h>
#include "../config.h"

typedef struct{
	int base;
	int limite;
	int libre;
	struct tm* lru;
	op_code cola;
	int32_t id;
	int32_t id_c;
	int tamanio_real;
}t_particion;

/*
particion_create crea una nueva particion si se le indica la base y el limite
guardando la hora en la que se creo dicha particion
*/
t_particion* particion_create(int base, int limite, int libre);

int particion_tamanio(t_particion* particion);

/*
particion_esta_libre devuelve verdadero si la particion puede guardar bytes y
falso si ya esta ocupada
*/
int particion_esta_libre(t_particion* particion);

/*
particion_combinar agranda la particion izquierda combinandola con la particion
derecha
*/
void particion_combinar(t_particion* particionIzq, t_particion* particionDer);

/*
particion_puede_guardarlo devuelve verdadero si la cantidad de bytes que se desean
guardar es menor o igual al espacio libre de la particion
*/
int particion_puede_guardarlo(t_particion* particion, int tamanio_a_guardar);

/*
particion_justa devuelve verdadero si la particion tiene el tamaño justo para guardar
la cantidad de bytes indicadas por parametro y falso si es mayor o menor
*/
int particion_justa(t_particion* particion, int tamanio_a_guardar);

/*
particion_ocuparla recibe una particion libre y la ocupa, luego de ocuparla crea la
nueva particion libre y la retorna. En caso de que el tamanio a ocupar sea igual que
el tamanio libre devuelve NULL indicando que no se creo ninguna particion libre nueva.
En caso de que el tamanio a ocupar se menor que el libre retorna una nueva particion
libre y modifica la que pasa por parametro dejandola ocupada
*/
t_particion* particion_ocuparla(t_particion* particion_libre, int tamanio_a_ocupar);

/*
particion_liberar hace que una particion se libere
*/
void particion_liberar(t_particion* particion);

/*
particion_son_iguales indica si dos particiones tiene los mismos atributos
*/
int particion_son_iguales(t_particion* particion1, t_particion* particion2);

/*
particion_destroy liberar la memoria usada al crear la particion
*/
void particion_destroy(t_particion* particion);

#endif /* MEMORIA_PARTICIONES_DINAMICAS_H_ */
