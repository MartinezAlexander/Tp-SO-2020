#include "entrenador.h"

t_entrenador* entrenador_create(char* posicion, char* pokemones, char* objetivos){
	t_entrenador* entrenador = malloc(sizeof(t_entrenador));

	char** posiciones_separadas = string_split(posicion, "|");
	entrenador->posicion.x = atoi(posiciones_separadas[0]);
	entrenador->posicion.y = atoi(posiciones_separadas[1]);

	/*
	 * Nos quedo duda de si aca hay que hacer un malloc a los char**
	 */
	entrenador->pokemones_adquiridos = string_split(pokemones, "|");
	entrenador->objetivos = string_split(objetivos, "|");
	entrenador->estado = NEW;

	return entrenador;
}


void entrenador_mostrar(t_entrenador* entrenador){
	printf("Mostrando entrenador: \n");
	printf("Posicion: %d,%d \n", entrenador->posicion.x, entrenador->posicion.y );
	printf("Objetivos: [");

	int i = 0;
	while(entrenador->objetivos[i] != NULL){
		printf(" %s ", entrenador->objetivos[i]);
		i++;
	}
	printf("] \n");

	printf("Pokemones: [");

	i = 0;
	while(entrenador->pokemones_adquiridos[i] != NULL){
		printf(" %s ", entrenador->pokemones_adquiridos[i]);
		i++;
	}
	printf("] \n");

	printf("Estado: %d \n\n", entrenador->estado);

}
