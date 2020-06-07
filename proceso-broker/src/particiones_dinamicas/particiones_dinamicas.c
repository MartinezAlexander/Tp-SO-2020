#include "particiones_dinamicas.h"

t_particion* particion_create(int base, int limite, int libre){
	t_particion* nueva_particion = malloc(sizeof(t_particion));
	nueva_particion->base = base;
	nueva_particion->limite = limite;
	nueva_particion->libre = libre;
	nueva_particion->lru = time(NULL);
	return nueva_particion;
}

int particion_tamanio(t_particion* particion){
	return particion->limite - particion->base;
}

int particion_esta_libre(t_particion* particion){
	return particion->libre;
}

void particion_combinar(t_particion* particionIzq, t_particion* particionDer){
	particionIzq->limite = particionDer->limite;
}

int particion_puede_guardarlo(t_particion* particion, int tamanio_a_guardar){
	return tamanio_a_guardar <= particion_tamanio(particion);
}

int particion_justa(t_particion* particion, int tamanio_a_guardar){
	return particion_tamanio(particion) == tamanio_a_guardar;
}

t_particion* particion_ocuparla(t_particion* particion_libre, int tamanio_a_ocupar){
	t_particion* nueva_particion_libre;
	if(particion_justa(particion_libre,tamanio_a_ocupar)){
		nueva_particion_libre = NULL;
		particion_libre->libre = 0;
		particion_libre->lru = time(NULL);
		//TODO Guardar en cache
	}else{
		particion_libre->libre = 0;
		particion_libre->lru = time(NULL);
		nueva_particion_libre = particion_create(particion_libre->base + tamanio_a_ocupar,particion_libre->limite,1);
		particion_libre->limite = particion_libre->base + tamanio_a_ocupar;
		//TODO Guardar en cache
	}

	return nueva_particion_libre;
}

void particion_liberar(t_particion* particion){
	particion->libre = 1;
}

int particion_son_iguales(t_particion* particion1, t_particion* particion2){
	return particion1->base == particion2->base && particion1->libre == particion2->libre && particion1->limite == particion2->limite && particion1->lru == particion2->lru;
}

void particion_destroy(t_particion* particion){
	free(particion);
}
