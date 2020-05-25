#include "procesamiento_appeared.h"

void descartar_pokemon(t_pokemon* pokemon){
	free(pokemon);
}

void procesar_appeared(t_appeared_pokemon* appeared_pokemon){
	printf("Procesando mensaje APPEARED\n");
	printf("Recibo %s\n\n", appeared_pokemon->pokemon->especie);

	int resultado = procesar_pokemon(appeared_pokemon->pokemon);

	if(resultado == 0){
		appeared_pokemon_destroy(appeared_pokemon);
	}
}

/**
 * Procesa un pokemon y devuelve en 1 caso de que planifique, 0 en caso contrario
 */
int procesar_pokemon(t_pokemon* pokemon){
	int cumplio_objetivos = cumplio_objetivo_global(objetivo_global, entrenadores);
	int necesito_pokemon = pokemon_dentro_de_objetivos(objetivo_global, pokemon->especie);

	int planifico = !cumplio_objetivos && necesito_pokemon;

	if(planifico){
		dictionary_put(diccionario_especies_recibidas, pokemon->especie, 1);
		entrenador_entrar_a_planificacion(pokemon);
	}else
		descartar_pokemon(pokemon);

	return planifico;
}
