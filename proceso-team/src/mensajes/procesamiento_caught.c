#include "procesamiento_caught.h"

void descartar_caught(t_caught_pokemon* mensaje){
	caught_pokemon_destroy(mensaje);
}

void procesar_caught(t_caught_pokemon* caught_pokemon, int32_t id_correlativo){
	char* key_id = string_itoa(id_correlativo);

	if(!dictionary_has_key(mensajes_catch_pendientes, key_id)){//Validar id mensaje
		descartar_caught(caught_pokemon);
	}else{
		t_entrenador* entrenador = dictionary_get(mensajes_catch_pendientes, key_id);

		if(caught_pokemon->atrapado)//Validar resultado del caught
			resolver_caught_positivo(entrenador);
		else
			resolver_caught_negativo(entrenador);

		dictionary_remove(mensajes_catch_pendientes, key_id);
	}
}

