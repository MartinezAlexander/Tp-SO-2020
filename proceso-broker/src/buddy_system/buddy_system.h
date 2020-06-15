#ifndef MEMORIA_BUDDY_SYSTEM_H_
#define MEMORIA_BUDDY_SYSTEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/string.h>
#include <mensajes/mensajes.h>
#include <math.h>
#include "../config.h"


typedef struct {
	int tamanio; //exponente de la potencia de 2
	int base; //direccion donde arranca la particion
	int libre;
	int tamanio_mensaje;
	op_code cola;
	int32_t id;
	int32_t id_c;
	void* padre;
	void* izq;
	void* der;
}t_buddy;

t_queue* buddies_victimas_fifo;

t_list* buddies_victimas_lru;


//recorrido en pre orden del arbol binario, le mando la lista de hojas VACIA
void recolectar_hojas_libres(t_buddy* raiz, t_list* hojas);
void recolectar_hojas_ocupadas(t_buddy* raiz, t_list* hojas);
//comparador para ordenar
bool menor_a_mayor(void* buddy1, void* buddy2);

t_buddy* buddy_create(int tamanio, int base);

//buddy_split agarra un buddy y crea sus dos hijos
void buddy_split(t_buddy* buddy);

//Dado el exponente te retorna el tamanio real que ocupa el buddy (2^exp)
int buddy_get_real_size(t_buddy* buddy);
int tamanio_ideal_para_el_buddy(t_mensaje* mensaje, t_buddy* buddy);
int mensaje_entra_en_buddy(t_mensaje* mensaje, t_buddy* buddy);

int buddy_esta_libre(t_buddy* buddy);
void buddy_ocupar(t_buddy* buddy, t_mensaje* mensaje);

//consoolida dos buddies libres y devuelve al padre libre o NULL si no es podible consolidar (hermano ocupado)
t_buddy* buddy_consolidar(t_buddy* buddy);
void buddy_liberar(t_buddy* buddy);

t_buddy* buddy_get_hijo_derecho(t_buddy* padre);
t_buddy* buddy_get_hijo_izquierdo(t_buddy* padre);
t_buddy* buddy_get_hermano(t_buddy* buddy);

int buddy_es_hoja(t_buddy* buddy);
int buddy_es_raiz(t_buddy* buddy);

void buddy_mostrar(t_buddy* buddy);

void destruir_hijo_derecho(t_buddy* padre);
void destruir_hijo_izquierdo(t_buddy* padre);
void buddy_destroy(t_buddy* buddy);

#endif /* MEMORIA_BUDDY_SYSTEM_H_ */
