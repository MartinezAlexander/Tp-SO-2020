#include "buddy_system.h"

t_buddy* buddy_create(int tamanio, int base){
	t_buddy* buddy = malloc(sizeof(t_buddy));
	buddy->tamanio = tamanio;
	buddy->libre = 1;
	buddy->base = base;
	buddy->izq = NULL;
	buddy->der = NULL;
	return buddy;
}

t_buddy* buddy_get_hijo_derecho(t_buddy* padre){
	return padre->der;
}

t_buddy* buddy_get_hijo_izquierdo(t_buddy* padre){
	return padre->izq;
}

void buddy_ocupar(t_buddy* buddy){
	buddy->libre = 0;
	/////
}

int buddy_es_hoja(t_buddy* buddy){
	return buddy->der == NULL && buddy->izq == NULL;
}

void buddy_split(t_buddy* buddy){
	if(buddy->tamanio <= 0){ //cambiar 0 por tamanio minimo de particion
		printf("No se puede dividir el buddy, tamanio minimo 1 byte\n");
		exit(1);
	}
	int tam_hijos = buddy->tamanio -1;

	int base_izq = buddy->base;
	t_buddy* izquierdo = buddy_create(tam_hijos, base_izq);
	buddy->izq = izquierdo;

	int base_der = base_izq^buddy_get_real_size(tam_hijos);
	t_buddy* derecho = buddy_create(tam_hijos, base_der);
	buddy->der = derecho;

}

int buddy_get_real_size(int exponente){
	return (int)pow((double)2,(double)exponente);
}

void buddy_destroy(t_buddy* buddy){
	if(buddy->der == NULL && buddy->izq == NULL){
		free(buddy);
	}else{
		puts("Liberar hijos antes de liberar al buddy");
	}
}

