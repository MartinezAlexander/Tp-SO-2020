#include "buddy_system.h"

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

void recolectar_hojas_ocupadas(t_buddy* raiz, t_list* hojas){
	if(buddy_es_hoja(raiz)){
		if(!buddy_esta_libre(raiz)){
			list_add(hojas, raiz);
		}
	}else{
		recolectar_hojas_ocupadas(raiz->izq,hojas);
		recolectar_hojas_ocupadas(raiz->der,hojas);
	}
}

void recolectar_hojas(t_buddy* raiz, t_list* hojas){
	if(buddy_es_hoja(raiz)){
		list_add(hojas, raiz);
	}else{
		recolectar_hojas(raiz->izq,hojas);
		recolectar_hojas(raiz->der,hojas);
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
	buddy->cola = 0;
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

void buddy_ocupar(t_buddy* buddy, t_mensaje* mensaje){
	buddy->libre = 0;
	buddy->cola = mensaje->codigo;
	buddy->id = mensaje->id;
	buddy->id_c = mensaje->id_correlativo;
	buddy->tamanio_mensaje = mensaje_size(mensaje);
	if(string_equals_ignore_case(algoritmo_reemplazo,"FIFO")){
		queue_push(buddies_victimas_fifo,buddy);
	}else{
		list_add(buddies_victimas_lru,buddy);
	}
}

void buddy_liberar(t_buddy* buddy){
	buddy->libre = 1;
	t_buddy* buddy_consolidado = buddy;
	do{
		buddy_consolidado = buddy_consolidar(buddy_consolidado);
	}while(buddy_consolidado != NULL);
}

t_buddy* buddy_consolidar(t_buddy* buddy){
	t_buddy* hermano = buddy_get_hermano(buddy);
	t_buddy* padre = NULL;
	if (hermano != NULL && buddy_esta_libre(hermano)){
		padre = buddy->padre;
		padre->libre = 1;
		destruir_hijo_derecho(padre);
		destruir_hijo_izquierdo(padre);
	}
	return padre;
}

int buddy_es_hoja(t_buddy* buddy){
	return buddy->der == NULL && buddy->izq == NULL;
}

int buddy_es_raiz(t_buddy* buddy){
	return buddy->padre == NULL;
}

void buddy_split(t_buddy* buddy){
	int tam_hijos = buddy->tamanio -1;

	buddy->libre = 0;

	int base_izq = buddy->base;
	t_buddy* izquierdo = buddy_create(tam_hijos, base_izq);
	izquierdo->padre = buddy;
	buddy->izq = izquierdo;

	int base_der = base_izq^buddy_get_real_size((t_buddy*)buddy->izq);
	t_buddy* derecho = buddy_create(tam_hijos, base_der);
	derecho->padre = buddy;
	buddy->der = derecho;
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

int mensaje_entra_en_buddy(t_mensaje* mensaje, t_buddy* buddy){
	return mensaje_size(mensaje) <= buddy_get_real_size(buddy);
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


void destruir_hijo_izquierdo(t_buddy* padre){
	free(padre->izq);
	padre->izq = NULL;
}

void destruir_hijo_derecho(t_buddy* padre){
	free(padre->der);
	padre->der = NULL;
}

void buddy_destroy(t_buddy* buddy){
	if(buddy_es_raiz(buddy)){
		destruir_hijo_derecho(buddy);
		destruir_hijo_izquierdo(buddy);
		free(buddy);
	}
}

