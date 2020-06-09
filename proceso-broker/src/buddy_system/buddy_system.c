#include "buddy_system.h"

void iniciar_buddy_system(){
	double exp = log2((double)tamano_memoria);
	int exp_int = (int)exp;

	if(exp - exp_int == 0){
		t_buddy* buddy = buddy_create(exp_int,0);

		//TODO cosas

	}
	else{
		puts("No se puede implementar un BS de un tamano que no sea potencia de 2");
		exit(1);
	}
}

t_list* obtener_mensajes_cacheados_por_cola_bs(op_code cola){
	//TODO esto
	return 0;
}

void bs_cachear_mensaje(t_mensaje* mensaje){
	//TODO esto
}



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

int buddy_esta_libre(t_buddy* buddy){
	return buddy->libre;
}


void buddy_ocupar(t_buddy* buddy, int tamanio_a_ocupar){
	if( buddy_get_real_size(buddy)>tamanio_a_ocupar && buddy_esta_libre(buddy)){
		buddy->libre = 0;
		buddy->tamanio_mensaje = tamanio_a_ocupar;
		//TODO guardar en cache
	}else{
		printf("No es posible ocupar el buddy con el tamanio %d\n", tamanio_a_ocupar);
	}
}

int buddy_es_hoja(t_buddy* buddy){
	return buddy->der == NULL && buddy->izq == NULL;
}

int buddy_es_raiz(t_buddy* buddy){
	return buddy->padre == NULL;
}

void buddy_split(t_buddy* buddy){
	if((buddy_get_real_size(buddy) >= (int)tamano_minimo_particion*2) && buddy_es_hoja(buddy)){
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

void poner_hijos_en_null(t_buddy* buddy){
	if(buddy_es_hoja(buddy->izq) && buddy_es_hoja(buddy->der)){
		buddy->izq = NULL;
		buddy->der = NULL;
	}else{
		puts("No puedo poner hijos en null, no son hojas!");
	}
}

void buddy_destroy(t_buddy* buddy){
	if(buddy_es_hoja(buddy)){
		free(buddy);
	}else{
		puts("Liberar hijos antes de liberar al buddy");
	}
}

void buddy_liberar(t_buddy* buddy){
	poner_hijos_en_null(buddy);
	buddy_destroy(buddy);
}
