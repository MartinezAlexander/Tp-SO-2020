#include "particiones_dinamicas.h"

t_particion* particion_create(int base, int limite, int libre){
	t_particion* nueva_particion = malloc(sizeof(t_particion));
	nueva_particion->base = base;
	nueva_particion->limite = limite;
	nueva_particion->libre = libre;
	time_t tiempo = time(NULL);
	nueva_particion->lru = localtime(&tiempo);
	nueva_particion->tamanio_real = limite - base;
	return nueva_particion;
}

int particion_tamanio(t_particion* particion){
	return particion->tamanio_real;
}

int particion_esta_libre(t_particion* particion){
	return particion->libre;
}

void particion_combinar(t_particion* particionIzq, t_particion* particionDer){
	particionIzq->limite = particionDer->limite;
}

int particion_puede_guardarlo(t_particion* particion, int tamanio_a_guardar){
	int puede_guardarlo = 0;

	if(tamanio_a_guardar < tamano_minimo_particion){
		if(tamano_minimo_particion <= particion_tamanio(particion)){
			puede_guardarlo = 1;
		}
	}else{
		if (tamanio_a_guardar <= particion_tamanio(particion)) {
			puede_guardarlo = 1;
		}
	}

	return puede_guardarlo;
}

int particion_justa(t_particion* particion, int tamanio_a_guardar){
	return particion_tamanio(particion) == tamanio_a_guardar;
}

t_particion* particion_ocuparla(t_particion* particion_libre, int tamanio_a_ocupar_real){

	int tamanio_a_ocupar = tamanio_a_ocupar_real;

	if(tamanio_a_ocupar_real < tamano_minimo_particion){
		tamanio_a_ocupar = tamano_minimo_particion;
	}

	t_particion* nueva_particion_libre;
	if(particion_justa(particion_libre,tamanio_a_ocupar)){
		nueva_particion_libre = NULL;
		particion_libre->libre = 0;
		time_t tiempo = time(NULL);
		particion_libre->lru = localtime(&tiempo);
	}else{
		particion_libre->libre = 0;
		time_t tiempo = time(NULL);
		particion_libre->lru = localtime(&tiempo);
		nueva_particion_libre = particion_create(particion_libre->base + tamanio_a_ocupar,particion_libre->limite,1);
		particion_libre->limite = particion_libre->base + tamanio_a_ocupar;
	}

	return nueva_particion_libre;
}

void particion_liberar(t_particion* particion){
	particion->libre = 1;
	particion->tamanio_real = particion->limite - particion->base;
}

int particion_son_iguales(t_particion* particion1, t_particion* particion2){
	return particion1->base == particion2->base && particion1->libre == particion2->libre && particion1->limite == particion2->limite && particion1->lru == particion2->lru;
}

void particion_destroy(t_particion* particion){
	free(particion);
}
