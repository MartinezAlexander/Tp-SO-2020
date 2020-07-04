#include "procesamiento_localized.h"

int especie_no_procesada(char* especie){
	return !dictionary_get(diccionario_especies_recibidas, especie);
}

//Para el localized voy a hacer llamada de la misma funcion de procesar que para el appeared.
//Solo que aca debo ver en mi diccionario de especies procesadas si alguien ya
//proceso un mensaje (appeared o localized) de la nueva especie
void procesar_localized(t_localized_pokemon* localized_pokemon){

	pthread_mutex_lock(&mutex_diccionario_especies);
	if(especie_no_procesada(localized_pokemon->nombre)){
		pthread_mutex_unlock(&mutex_diccionario_especies);

		t_list* pokemones = localized_pokemon_get_list(localized_pokemon);

		//Por cada uno en la lista proceso como appeared
		for(int i = 0 ; i< list_size(pokemones) ; i++){
			t_pokemon* pokemon = list_get(pokemones, i);
			procesar_pokemon(pokemon);
		}
	}else{
		pthread_mutex_unlock(&mutex_diccionario_especies);
	}

	//Como ya tengo el localized convertido a una lista de pokemones,
	//puedo liberar los recursos del mensaje, que se hace de esta manera particular
	posiciones_destroy_elements(localized_pokemon->posiciones);
	localized_pokemon_destroy(localized_pokemon);
}
