#include "buddy_system.h"

int tamanio_minimo_particion = 32;

void recolectar_hojas_libres(t_buddy* raiz, t_list* hojas){
	if(buddy_es_hoja(raiz)){
		if(buddy_esta_libre(raiz)){
			list_add(hojas, raiz);
		}
	}else{
		recolectar_hojas_libres(raiz->izq,hojas);
		recolectar_hojas_libres(raiz->der,hojas);
	}
}

bool menor_a_mayor(void* buddy1, void* buddy2){
	return ((t_buddy*)buddy1)->tamanio <= ((t_buddy*)buddy2)->tamanio;
}

t_buddy* buddy_create(int tamanio, int base){
	t_buddy* buddy = malloc(sizeof(t_buddy));
	buddy->tamanio = tamanio;
	buddy->libre = 1;
	buddy->base = base;
	buddy->tamanio_mensaje = 0;
	buddy->padre = NULL;
	buddy->izq = NULL;
	buddy->der = NULL;
	return buddy;
}

t_buddy* buddy_get_hijo_derecho(t_buddy* padre){
	return (t_buddy*)padre->der;
}

t_buddy* buddy_get_hijo_izquierdo(t_buddy* padre){
	return (t_buddy*)padre->izq;
}

t_buddy* buddy_get_hermano(t_buddy* buddy){
	t_buddy* hermano = NULL;
	if(!buddy_es_raiz(buddy)){
		if(((t_buddy*)buddy->padre)->der == buddy){
			hermano = ((t_buddy*)buddy->padre)->izq;
		}else{
			hermano = ((t_buddy*)buddy->padre)->der;
		}
	}
	return hermano;
}


int buddy_esta_libre(t_buddy* buddy){
	return buddy->libre;
}


void buddy_ocupar(t_buddy* buddy, int tamanio_a_ocupar){
	buddy->libre = 0;
	buddy->tamanio_mensaje = tamanio_a_ocupar;
}

int buddy_es_hoja(t_buddy* buddy){
	return buddy->der == NULL && buddy->izq == NULL;
}

int buddy_es_raiz(t_buddy* buddy){
	return buddy->padre == NULL;
}

void buddy_split(t_buddy* buddy){
	if((buddy_get_real_size(buddy) >= (int)tamanio_minimo_particion*2) && buddy_es_hoja(buddy)){
		int tam_hijos = buddy->tamanio -1;

		int base_izq = buddy->base;
		t_buddy* izquierdo = buddy_create(tam_hijos, base_izq);
		izquierdo->padre = buddy;
		buddy->izq = izquierdo;

		int base_der = base_izq^buddy_get_real_size((t_buddy*)buddy->izq);
		t_buddy* derecho = buddy_create(tam_hijos, base_der);
		derecho->padre = buddy;
		buddy->der = derecho;
	}else{
		puts("No puedo partir el buddy");
	}
}

int int_pow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp % 2)
           result *= base;
        exp /= 2;
        base *= base;
    }
    return result;
}

int buddy_get_real_size(t_buddy* buddy){
	return int_pow(2,buddy->tamanio);
}

int tamanio_ideal_para_el_buddy(t_mensaje* mensaje, t_buddy* buddy){
	return mensaje_size(mensaje)<=buddy_get_real_size(buddy)  &&  mensaje_size(mensaje) > buddy_get_real_size(buddy)/2;
}


void buddy_mostrar(t_buddy* buddy){
	printf("Tamanio: %d\n",buddy_get_real_size(buddy));
	printf("Base: %d\n",buddy->base);

	if(buddy_esta_libre(buddy)){
		puts("Libre");
	}else{
		puts("Ocupado");
	}

	if(!buddy_es_raiz(buddy)){
		puts("Tiene padre");
	}else{
		puts("Es raiz");
	}

	if(buddy_es_hoja(buddy)){
		puts("Es hoja");
	}else{
		printf("Tamanio hijo izq: %d\n",buddy_get_real_size(buddy->izq));
		printf("Tamanio hijo der: %d\n",buddy_get_real_size(buddy->der));
	}
	puts("-------------------------------");

}

void liberar_hijo_izquierdo(t_buddy* padre){
	free(padre->izq);
	padre->izq = NULL;
}

void liberar_hijo_derecho(t_buddy* padre){
	free(padre->der);
	padre->der = NULL;
}

void buddy_destroy(t_buddy* buddy){
	if(buddy_es_raiz(buddy)){
		liberar_hijo_derecho(buddy);
		liberar_hijo_izquierdo(buddy);
		free(buddy);
	}
}

