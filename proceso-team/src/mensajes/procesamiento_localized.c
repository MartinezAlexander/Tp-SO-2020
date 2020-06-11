#include "procesamiento_localized.h"

int especie_no_procesada(char* especie){
	return !dictionary_get(diccionario_especies_recibidas, especie);
}

void procesar_localized(t_localized_pokemon* localized_pokemon){

	pthread_mutex_lock(&mutex_diccionario_especies);
	if(especie_no_procesada(localized_pokemon->nombre)){
		pthread_mutex_unlock(&mutex_diccionario_especies);

		t_list* pokemones = localized_pokemon_get_list(localized_pokemon);
		posiciones_destroy_elements(localized_pokemon->posiciones);
		localized_pokemon_destroy(localized_pokemon);

		//Por cada uno en la lista proceso como appeared
		for(int i = 0 ; i< list_size(pokemones) ; i++){
			t_pokemon* pokemon = list_get(pokemones, i);
			procesar_pokemon(pokemon);
		}
	}else{
		pthread_mutex_unlock(&mutex_diccionario_especies);
		posiciones_destroy_elements(localized_pokemon->posiciones);
		localized_pokemon_destroy(localized_pokemon);
	}
}
