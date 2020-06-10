#include "objetivos.h"

t_dictionary* inicializar_diccionario_especies(){
	t_dictionary* diccionario = dictionary_create();

	for(int i = 0 ; i < list_size(objetivo_global) ; i++){
		dictionary_put(diccionario, list_get(objetivo_global, i), 0);
	}

	return diccionario;
}

t_list* obtener_objetivo_global(t_list* entrenadores){
	t_list* objetivos_globales = list_create();
	t_list* adquiridos_globales = list_create();
	//Obtengo todos los objetivos y todos los adquiridos en dos listas
	for(int i = 0; i < list_size(entrenadores); i++){
		t_entrenador* entrenador = list_get(entrenadores, i);
		list_add_all(objetivos_globales, entrenador->objetivos);
		list_add_all(adquiridos_globales, entrenador->pokemones_adquiridos);
	}

	list_sort(objetivos_globales, strcmp);

	//Por cada elemento de los adquiridos, lo borro de la lista de objetivos
	for(int i = 0 ; i < list_size(adquiridos_globales) ; i++){
		char* pokemon = list_get(adquiridos_globales, i);

		sacar_de_objetivos_globales(pokemon, objetivos_globales);
	}
	list_destroy(adquiridos_globales);
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

	//Si no tienen mismo tamaño ya se que no son iguales
	if(list_size(adquiridos) != list_size(objetivo_global)) return 0;

	//La ordeno para poder compararlas
	list_sort(adquiridos, strcmp);

	//Comparo con los objetivos
	for(int i = 0 ; i < list_size(adquiridos) ; i++){
		char* pk1 = list_get(adquiridos, i);
		char* pk2 = list_get(objetivo_global, i);

		if(string_equals_ignore_case(pk1,pk2) != 1) return 0;
	}

	return 1;
}
