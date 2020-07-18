#include "procesamiento_caught.h"

void descartar_caught(t_caught_pokemon* mensaje){
	caught_pokemon_destroy(mensaje);
}

void procesar_caught(t_caught_pokemon* caught_pokemon, int32_t id_correlativo){
	//TODO MEMORY LEAK
	char* key_id = string_itoa(id_correlativo);

	//Valido id de mensaje
	if(!dictionary_has_key(mensajes_catch_pendientes, key_id)){
		descartar_caught(caught_pokemon);
		printf("[Caught] Descartado mensaje caught no correspondiente\n");
	}else{
		t_entrenador* entrenador = dictionary_get(mensajes_catch_pendientes, key_id);

		if(caught_pokemon->atrapado)//Valido resultado del caught
			resolver_caught_positivo(entrenador,1);
		else
			resolver_caught_negativo(entrenador);

		dictionary_remove(mensajes_catch_pendientes, key_id);
	}
}

