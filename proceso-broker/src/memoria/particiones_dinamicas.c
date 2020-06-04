#include "particiones_dinamicas.h"

t_particion* particion_create(int base, int limite, int libre){
	t_particion* nueva_particion = malloc(sizeof(t_particion));
	nueva_particion->base = base;
	nueva_particion->limite = limite;
	nueva_particion->libre = libre;
	nueva_particion->lru = time(NULL);
	return nueva_particion;
}

int particion_esta_libre(t_particion* particion){
	return particion->libre;
}

t_particion* particion_combinar(t_particion* particion1, t_particion* particion2){
	int nuevo_limite = particion1->limite + particion2->limite;
	int nueva_base;

	if(particion1->base <= particion2->base){
		nueva_base = particion1->base;
	}else{
		nueva_base = particion2->base;
	}

	return particion_create(nueva_base,nuevo_limite,1);

	//TODO liberar memoria de las dos particiones que llegan por parametro
}

int particion_puede_guardarlo(t_particion* particion, int tamanio_a_guardar){
	return tamanio_a_guardar <= particion->limite;
}

int particion_justa(t_particion* particion, int tamanio_a_guardar){
	return particion->limite == tamanio_a_guardar;
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
		int nuevo_limite_libre = particion_libre->limite - tamanio_a_ocupar;
		particion_libre->limite = tamanio_a_ocupar;
		particion_libre->lru = time(NULL);
		nueva_particion_libre = particion_create(particion_libre->limite+1,nuevo_limite_libre,1);
		//TODO Guardar en cache
	}

	return nueva_particion_libre;
}

void particion_destroy(t_particion* particion){
	free(particion);
}
