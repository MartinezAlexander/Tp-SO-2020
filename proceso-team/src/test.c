#include"test.h"

void test(){
	((t_entrenador*)list_get(entrenadores, 0))->estado_sjf->ultima_rafaga = 3;
	((t_entrenador*)list_get(entrenadores, 2))->estado_sjf->ultima_rafaga = 4;

	t_appeared_pokemon* nuevo_appeared = appeared_pokemon_create("pikachu", 3, 10);
	entrenador_entrar_a_planificacion(nuevo_appeared->pokemon);
	t_appeared_pokemon* nuevo_appeared2 = appeared_pokemon_create("charmander", 9, 6);
	entrenador_entrar_a_planificacion(nuevo_appeared2->pokemon);
	t_appeared_pokemon* nuevo_appeared3 = appeared_pokemon_create("pidgey", 2, 4);
	entrenador_entrar_a_planificacion(nuevo_appeared3->pokemon);
}

//Entrenadores en [1,2], [3,7], [5,5]
//Pikachu en [3,10] => mas cercano es entrenador 2
//Charmander en [9,6] => mas cercano es entrenador 3
//Pidgey en [2,4] => unico entrenador es entrenador 1

//Estimacion inicial = 2

//Rafagas anteriores (para verificar que ande bien)
//Entrenador 1 => raf anterior = 3
//Entrenador 3 => raf anterior = 4

//Arranca y planifica a 2
//Ultima estimacion de 2 = 1
//Hace 3 rafagas
//Ultima rafaga de 2 = 3

//Replanifica

//Calcula:
//Ultima estimacion de 1 = 2.5
//Ultima estimacion de 3 = 3

//Elige a entrenador 1
//Hace 3 rafagas
//Ultima rafaga de 1 = 3

//Replanifica

//Elige a 3 unico en cola
//Ejecuta
