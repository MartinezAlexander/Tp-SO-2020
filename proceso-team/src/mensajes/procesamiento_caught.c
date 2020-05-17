#include "procesamiento_caught.h"

void descartar_caught(t_caught_pokemon* mensaje){
	//TODO
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

void resolver_caught_positivo(t_entrenador* entrenador){
	entrenador_atrapar_objetivo(entrenador);

	//Actualizo el estado del entrenador
	if(cumplio_objetivo_entrenador(entrenador)){
		entrenador->estado = EXIT;
	}else{
		if(entrenador_estado_deadlock(entrenador))
			entrenador->estado = BLOCKED_DEADLOCK;
		else
			encolar(entrenador);//Caso si puedo seguir atrapando
	}
}

void resolver_caught_negativo(t_entrenador* entrenador){
	entrenador_resetear_objetivo(entrenador);
	entrenador->estado = BLOCKED;
}
