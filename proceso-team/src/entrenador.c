#include "entrenador.h"

t_entrenador* entrenador_create(char* posicion, char* pokemones, char* objetivos){
	t_entrenador* entrenador = malloc(sizeof(t_entrenador));

	char** posiciones_separadas = string_split(posicion, "|");
	entrenador->posicion.x = atoi(posiciones_separadas[0]);
	entrenador->posicion.y = atoi(posiciones_separadas[1]);

	/*
	 * Nos quedo duda de si aca hay que hacer un malloc a los char**
	 */
	char** adquiridos_array = string_split(pokemones, "|");
	entrenador->pokemones_adquiridos = array_to_list(adquiridos_array);
	char** objetivos_array = string_split(objetivos, "|");
	entrenador->objetivos = array_to_list(objetivos_array);
	entrenador->estado = NEW;

	return entrenador;
}

t_list* array_to_list(char** adquiridos){
	t_list* lista = list_create();
	for(int i=0; adquiridos[i]!=NULL; i++){
		list_add(lista,adquiridos[i]);
	}
	return lista;
}

void entrenador_mostrar(t_entrenador* entrenador){
	printf("Mostrando entrenador: \n");
	printf("Posicion: %d,%d \n", entrenador->posicion.x, entrenador->posicion.y );
	printf("Objetivos: [");


	for(int i=0; i<list_size(entrenador->objetivos); i++){
		printf(" %s ", list_get(entrenador->objetivos,i));
	}

	printf("] \n");

	printf("Pokemones: [");

	for(int i=0; i<list_size(entrenador->pokemones_adquiridos); i++){
		printf(" %s ", list_get(entrenador->pokemones_adquiridos,i));
	}

	printf("] \n");

	printf("Estado: %d \n\n", entrenador->estado);

}
