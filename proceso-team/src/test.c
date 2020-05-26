#include"test.h"

void test(){
	t_appeared_pokemon* nuevo_appeared = appeared_pokemon_create("pikachu", 3, 9);
	entrenador_entrar_a_planificacion(nuevo_appeared->pokemon);
	t_appeared_pokemon* nuevo_appeared2 = appeared_pokemon_create("charmander", 10, 15);
	entrenador_entrar_a_planificacion(nuevo_appeared2->pokemon);
}
