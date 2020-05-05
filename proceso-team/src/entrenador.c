#include "entrenador.h"

t_entrenador* entrenador_create(char* posicion, char* pokemones, char* objetivos){
	t_entrenador* entrenador = malloc(sizeof(t_entrenador));

	char** posiciones_separadas = string_split(posicion, "|");
	entrenador->posicion = *posicion_create( atoi(posiciones_separadas[0]) , atoi(posiciones_separadas[1]) );
	/*entrenador->posicion.posicionX = atoi(posiciones_separadas[0]);
	entrenador->posicion.posicionY = atoi(posiciones_separadas[1]);*/

	entrenador->pokemones_adquiridos = array_to_list(string_split(pokemones, "|"));
	entrenador->objetivos = array_to_list(string_split(objetivos, "|"));
	entrenador->estado = NEW;

	return entrenador;
}

t_list* array_to_list(char** adquiridos){

	t_list* lista = list_create();
	for(int i = 0; adquiridos[i]!=NULL ; i++){
		list_add(lista,adquiridos[i]);
	}
	return lista;
}


t_list* leer_entrenadores(t_config* config){
	char** posiciones_entrenadores = config_get_array_value(config, "POSICIONES_ENTRENADORES");
	char** pokemones_entrenadores = config_get_array_value(config, "POKEMON_ENTRENADORES");
	char** objetivos_entrenadores = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");

	int numero_posiciones = cantidad_de_elementos(posiciones_entrenadores);
	int numero_pok_entrenadores = cantidad_de_elementos(pokemones_entrenadores);
	int numero_obj_entrenadores = cantidad_de_elementos(objetivos_entrenadores);

	//Error si no coinciden las cantidades
	if(numero_posiciones != numero_pok_entrenadores
			|| numero_pok_entrenadores != numero_obj_entrenadores){
		printf("Error: no coindiden las cantidades de pos-pok-obj de entrenadores en config!");
	}

	t_list* entrenadores = list_create();
	for(int i = 0 ; i < numero_posiciones ; i++){
		list_add(entrenadores,entrenador_create(posiciones_entrenadores[i],
				pokemones_entrenadores[i],
				objetivos_entrenadores[i]));
	}

	return entrenadores;
}


int cantidad_de_elementos(char** array){
	int cantidad = 0;
	while(array[cantidad] != NULL) cantidad++;
	return cantidad;
}


void entrenador_mostrar(t_entrenador* entrenador){
	printf("Mostrando entrenador: \n");
	printf("Posicion: %d,%d \n", *posicion_get_X(&entrenador->posicion), *posicion_get_Y(&entrenador->posicion));
	printf("Objetivos: [");


	for(int i=0; i<list_size(entrenador->objetivos); i++){
		char* objetivos = (char*)list_get(entrenador->objetivos,i);
		printf(" %s ", objetivos);
	}

	printf("] \n");

	printf("Pokemones: [");

	for(int i=0; i<list_size(entrenador->pokemones_adquiridos); i++){
		char* pokemones = (char*)list_get(entrenador->pokemones_adquiridos,i);
		printf(" %s ", pokemones);
	}

	printf("] \n");

	printf("Estado: %d \n\n", entrenador->estado);

}
