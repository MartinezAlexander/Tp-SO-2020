#include "procesamiento_appeared.h"

void descartar_pokemon(t_pokemon* pokemon){
	free(pokemon);
}

void procesar_appeared(t_appeared_pokemon* appeared_pokemon){
	printf("Procesando mensaje APPEARED\n");
	printf("Recibo %s\n\n", appeared_pokemon->pokemon->especie);

	procesar_pokemon(appeared_pokemon->pokemon);

}

/**
 * Procesa un pokemon y devuelve en 1 caso de que planifique, 0 en caso contrario
 */
void procesar_pokemon(t_pokemon* pokemon){

	int planifico = pokemon_dentro_de_objetivos(objetivo_global, pokemon->especie);

	if(planifico){
		pthread_mutex_lock(&mutex_diccionario_especies);
		dictionary_put(diccionario_especies_recibidas, pokemon->especie, 1);
		pthread_mutex_unlock(&mutex_diccionario_especies);

		sacar_de_objetivos_globales(pokemon->especie, objetivo_global);

		entrenador_entrar_a_planificacion(pokemon);
	}else{
		//Antes de descartar me tengo que fijar si alguien ya esta
		//yendo a buscar uno de la misma especie. En ese caso, este me lo voy
		//a guardar por si ese falla.

		//Entonces me fijo si algun entrenador lo tiene asignado como
		//objetivo actual
		for(int i = 0 ; i < list_size(entrenadores) ; i++){
			t_entrenador* entrenador = list_get(entrenadores, i);
			//En caso de que este asignado, lo agrego a la cola de pendientes
			if(entrenador_tiene_objetivo(entrenador, pokemon->especie)){
				queue_push(cola_pokemones_en_espera, pokemon);
				puts("[Pokemon] Pokemon puesto en espera, motivo: repuesto\n");
				return;
			}
		}

		//Lo pongo en Cola de pokemones en espera
		descartar_pokemon(pokemon);
	}


}

void procesar_pokemon_en_espera(){
	if(!queue_is_empty(cola_pokemones_en_espera)){
		t_pokemon* pokemon = queue_pop(cola_pokemones_en_espera);
		puts("[Pokemon] Procesamiento pokemon en espera, motivo: se libero un entrenador\n");
		procesar_pokemon(pokemon);
	}
}
