#include "procesamiento_appeared.h"

void descartar_appeared(t_appeared_pokemon* appeared_pokemon){
	//TODO
}

void procesar_appeared(t_appeared_pokemon* appeared_pokemon){
	printf("Procesando mensaje APPEARED\n");
	printf("Recibo %s\n\n", appeared_pokemon->pokemon->especie);

	int cumplio_objetivos = cumplio_objetivo_global(objetivo_global, entrenadores);
	int necesito_pokemon = pokemon_dentro_de_objetivos(objetivo_global, appeared_pokemon->pokemon->especie);

	if(cumplio_objetivos || !necesito_pokemon)
		descartar_appeared(appeared_pokemon);
	else
		entrenador_entrar_a_planificacion(appeared_pokemon->pokemon);
}
