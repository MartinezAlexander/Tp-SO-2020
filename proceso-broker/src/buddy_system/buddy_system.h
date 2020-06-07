#ifndef MEMORIA_BUDDY_SYSTEM_H_
#define MEMORIA_BUDDY_SYSTEM_H_
/*
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct {
	int tamanio; //exponente de la potencia de 2
	int base; //direccion donde arranca la particion
	int libre;
	struct t_buddy *izq;
	struct t_buddy *der;
}t_buddy;


 buddy_create crea la particion inicial de tamanio fijo y de ahi voy dividiendo en buddies
 inicializo en null los buddies hijos

t_buddy* buddy_create(int tamanio, int base);

 buddy_split agarra un buddy y crea sus dos hijos

void buddy_split(t_buddy* buddy);


 buddy_get_buddy devuelve el buddy del que le mando

t_buddy* buddy_get_buddy(t_buddy* buddy);


Dado el exponente te retorna el tamanio real que ocupa el buddy (2^exp)

int buddy_get_real_size(int exponente);

t_buddy* buddy_get_hijo_derecho(t_buddy* padre);
t_buddy* buddy_get_hijo_izquierdo(t_buddy* padre);


 Destruye el buddy

void buddy_destroy(t_buddy* buddy);*/

#endif /* MEMORIA_BUDDY_SYSTEM_H_ */
