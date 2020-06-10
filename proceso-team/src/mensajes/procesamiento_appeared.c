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
	}else
		descartar_pokemon(pokemon);

}
