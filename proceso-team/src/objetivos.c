#include "objetivos.h"

t_list* obtener_objetivo_global(t_list* entrenadores){
	t_list* objetivos_globales = list_create();

	for(int i = 0; i < list_size(entrenadores); i++){
		t_entrenador* entrenador = list_get(entrenadores, i);
		list_add_all(objetivos_globales, entrenador->objetivos);
	}

	list_sort(objetivos_globales, strcmp);

	return objetivos_globales;
}

int pokemon_dentro_de_objetivos(t_list* objetivos, char* pokemon){
	for(int i = 0 ; i < list_size(objetivos); i++){
		if(string_equals_ignore_case(list_get(objetivos, i),pokemon) == 1)
			return 1;
	}
	return 0;
}

int cumplio_objetivo_global(t_list* objetivo_global, t_list* entrenadores){

	//Junto todos los adquiridos en una lista
	t_list* adquiridos = list_create();
	for(int i = 0 ; i < list_size(entrenadores) ; i++){

		t_entrenador* entrenador = list_get(entrenadores, i);
		list_add_all(adquiridos, entrenador->pokemones_adquiridos);
	}

	//La ordeno para poder compararlas
	list_sort(adquiridos, strcmp);

	//Comparo con los objetivos
	for(int i = 0 ; i < list_size(adquiridos) ; i++){
		char* pk1 = list_get(adquiridos, i);
		char* pk2 = list_get(objetivo_global, i);

		if(string_equals_ignore_case(pk1,pk2) != 1) return 0;
	}

	//En caso de que tenga una lista de adquiridos mas corta y se de que
	//todos los elementos sean iguales a los primeros de los objetivos
	//(todavia no cumplio el objetivo), verifico que sean del mismo tamaño
	//para asegurarme de que no pase esto
	return list_size(adquiridos) == list_size(objetivo_global);
}
