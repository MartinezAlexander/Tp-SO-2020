#ifndef MEMORIA_BUDDY_SYSTEM_H_
#define MEMORIA_BUDDY_SYSTEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <mensajes/mensajes.h>
#include <math.h>
#include "../config.h"

typedef struct {
	int tamanio; //exponente de la potencia de 2
	int base; //direccion donde arranca la particion
	int libre;
	int tamanio_mensaje;
	void* padre;
	void* izq;
	void* der;
}t_buddy;


//recorrido en pre orden del arbol binario, le mando la lista de hojas VACIA
void recolectar_hojas_libres(t_buddy* raiz, t_list* hojas);

t_buddy* buddy_create(int tamanio, int base);

//buddy_split agarra un buddy y crea sus dos hijos
void buddy_split(t_buddy* buddy);

//Dado el exponente te retorna el tamanio real que ocupa el buddy (2^exp)
int buddy_get_real_size(t_buddy* buddy);

int buddy_esta_libre(t_buddy* buddy);
void buddy_ocupar(t_buddy* buddy, int tamanio_a_ocupar);

t_buddy* buddy_get_hijo_derecho(t_buddy* padre);
t_buddy* buddy_get_hijo_izquierdo(t_buddy* padre);

int buddy_es_hoja(t_buddy* buddy);
int buddy_es_raiz(t_buddy* buddy);

void buddy_mostrar(t_buddy* buddy);

void poner_hijos_en_null(t_buddy* buddy);
void buddy_destroy(t_buddy* buddy);
//buddy_liberar primero pone a los hijos en null y despues destruye el buddy
void buddy_liberar(t_buddy* buddy);

#endif /* MEMORIA_BUDDY_SYSTEM_H_ */
