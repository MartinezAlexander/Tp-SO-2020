#include "test_planificacion.h"

//Nota: los tests estan planteados para el archivo config de prueba que nos dieron
//POSICIONES_ENTRENADORES=[ 2|3, 6|5, 9|9 ]
//POKEMON_ENTRENADORES=[ pikachu, , ]
//OBJETIVOS_ENTRENADORES=[ pikachu|squirtle, pikachu|gengar, squirtle|onix ]
//ESTIMACION_INICIAL=5
//ALPHA=0.5
//QUANTUM=0

//POK_QUE_NECESITO = [ pikachu, squirtle (x2), gengar, onix ]

void test_fifo(){

}

void test_round_robin(){

}

void test_sjf_sin_desalojo(){
	//Aca no hay mucho que probar la verdad, solamente que se haga el calculo
	//correctamente, y que elija al menor

	((t_entrenador*)list_get(entrenadores, 0))->estado_sjf->ultima_rafaga = 3;
	((t_entrenador*)list_get(entrenadores, 1))->estado_sjf->ultima_rafaga = 9;
	((t_entrenador*)list_get(entrenadores, 2))->estado_sjf->ultima_rafaga = 4;

	t_pokemon* pikachu = pokemon_create("pikachu", posicion_create(3, 10));
	procesar_pokemon(pikachu);
	t_pokemon* squirtle = pokemon_create("squirtle", posicion_create(9, 6));
	procesar_pokemon(squirtle);
	t_pokemon* gengar = pokemon_create("gengar", posicion_create(2, 4));
	procesar_pokemon(gengar);

	//Entrenadores en [2,3], [6,5], [9,9]
	//Pikachu en [3,10] => mas cercano es entrenador 3
	//Squirtle en [9,6] => mas cercano es entrenador 2
	//Gengar en [2,4] => unico entrenador es entrenador 1

	//Planifico al entrenador (unico en cola)
	//Estimacion entrenador 3 => u_est = 5 ; u_raf = 4 => estimacion = 4,5
	//Elijo entrenador 3

	//Entrenador 3 se mueve hasta [3,10]

	//Planifico proximo entrenador [entrenador 2, entrenador 1]
	//Estimacion entrenador 2 => u_est = 5 ; u_raf = 9 => estimacion = 7
	//Estimacion entrenador 1 => u_est = 5 ; u_raf = 3 => estimacion = 4
	//Elijo entrenador 1

	//Entrenador 1 se mueve hasta [9,6]

	//Planifico al entrenador 2 (unico en cola)
	//Estimacion entrenador 2 => u_est = 5 ; u_raf = 9 => estimacion = 7
	//Elijo entrenador 2

	//Entrenador 2 se mueve hasta [2,4]
}

void test_sjf_con_desalojo(){
	//Aca hay que probar dos cosas. Con la prueba anterior ya sabemos que el
	//calculo normal anda bien, asi que hay que probar, primero que replanifique
	//cuando entre alguien nuevo a la cola (para esto voy a meter un sleep entre
	//los nuevos pokemones para darle un toque de tiempo a que ejecute el actual;
	//y ademas le metemos distancias largas asi probamos bien)
	//Y lo segundo que hay que probar es que se decida bien que calculo realizar
	//si el normal o el resto.

	((t_entrenador*)list_get(entrenadores, 0))->estado_sjf->ultima_rafaga = 3;
	((t_entrenador*)list_get(entrenadores, 1))->estado_sjf->ultima_rafaga = 9;
	((t_entrenador*)list_get(entrenadores, 2))->estado_sjf->ultima_rafaga = 4;

	t_pokemon* pikachu = pokemon_create("pikachu", posicion_create(4, 15));
	procesar_pokemon(pikachu);

	sleep(3);

	t_pokemon* squirtle = pokemon_create("squirtle", posicion_create(9, 6));
	procesar_pokemon(squirtle);

	sleep(3);

	t_pokemon* gengar = pokemon_create("gengar", posicion_create(2, 4));
	procesar_pokemon(gengar);

	sleep(3);
	t_pokemon* gengar2 = pokemon_create("gengar", posicion_create(5, 4));
	procesar_pokemon(gengar2);

	//Entrenadores en [2,3], [6,5], [9,9]
	//Pikachu en [4,15] => mas cercano es entrenador 3

	//Planifico al entrenador 3 (unico en cola)
	//Estimacion entrenador 3 => u_est = 5 ; u_raf = 4 => estimacion = 4,5
	//Elijo entrenador 3

	//Entrenador 3 se mueve a [8,9] 	(rafaga 1)
	//Entrenador 3 se mueve a [7,9]		(rafaga 2)
	//Entrenador 3 se mueve a [6,9]		(rafaga 3)

	//Squirtle en [9,6] => mas cercano es entrenador 2

	//Planifico proximo entrenador [entrenador 2 en cola, entrenador 3 ejecutando]
	//Estimacion entrenador 3 => u_est = 4.5 ; recorrido = 3 => resta = 1,5
	//Estimacion entrenador 2 => u_est = 5 ; u_raf = 9 => estimacion = 7
	//Elijo entrenador 3

	//Entrenador 3 se mueve a [5,9] 	(rafaga 4)
	//Entrenador 3 se mueve a [4,9]		(rafaga 5)
	//Entrenador 3 se mueve a [4,10]	(rafaga 6)

	//Gengar en [2,4] => unico entrenador es entrenador 1

	//Planifico proximo entrenador [entrenador 2, entrenador 1, entrenador 3 ejecutando]
	//Estimacion entrenador 3 => u_est = 4.5 ; recorrido = 6 => resta = -2,5
	//Estimacion entrenador 2 => u_est = 5 ; u_raf = 9 => estimacion = 7
	//Estimacion entrenador 1 => u_est = 5 ; u_raf = 3 => estimacion = 4
	//Elijo entrenador 3

	//Entrenador 1 se mueve hasta [4,15]

	//Planifico proximo entrenador [entrenador 2, entrenador 1, entrenador 3 ejecutando]
	//Estimacion entrenador 2 => u_est = 5 ; u_raf = 9 => estimacion = 7
	//Estimacion entrenador 1 => u_est = 5 ; u_raf = 3 => estimacion = 4
	//Elijo entrenador 1

	//Entrenador 1 se mueve hasta [9,6]

	//Planifico al entrenador 2 (unico en cola)
	//Estimacion entrenador 2 => u_est = 5 ; u_raf = 9 => estimacion = 7
	//Elijo entrenador 2

	//Entrenador 2 se mueve hasta [2,4]
}
