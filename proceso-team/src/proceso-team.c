/*
 ============================================================================
 Name        : proceso-team.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "proceso-team.h"

int main(void) {

	t_config* config;

	config = leer_config();

	t_list* entrenadores = leer_entrenadores(config);

	entrenador_mostrar(list_get(entrenadores,0));
	entrenador_mostrar(list_get(entrenadores,1));
	entrenador_mostrar(list_get(entrenadores,2));

	t_list* objetivos_globales = sumar_objetivos(entrenadores);

	// list_sort(objetivos_globales, strcmp());  TODO
    // No supe usar list_sort()

	for(int i=0; i<list_size(objetivos_globales); i++){
		printf("%s\n", list_get(objetivos_globales,i));
	}

	if(config != NULL)
	{
		config_destroy(config);
	}
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

t_list* sumar_objetivos(t_list* entrenadores){
	t_list* objetivos_globales = list_create();
	for(int x=0; x<list_size(entrenadores); x++){
		t_entrenador* entrenador = list_get(entrenadores, x);
		list_add_all(objetivos_globales, entrenador->objetivos);
		}
return objetivos_globales;
}

t_config* leer_config(void)
{
	t_config *config;//Puede que este mal el path
	if((config = config_create("src/team.config")) == NULL)
	{
		printf("No pude leer la config\n");
		exit(2);
	}
	return config;
}
