#include "test_deadlock.h"


/*
 * A tener en cuenta:
 * Por algun motivo cuando ordenamos lista de strings, se ordena de mayor a menor
 * asi que cuando pensemos los test tengamos en cuenta que el primero en la lista
 * va a ser el mayor
 */

void test_deadlock(){

	/*
	 * Para testear, la mejor forma es cambiando el config. Dejamos el que usamos para
	 * esta prueba:
	 *
	 *	Basado en las pruebas que nos da la catedra, este caso es un deadlock
	 *	de espera circular
	 *
	 * 	POSICIONES_ENTRENADORES=[2|2,4|6,10|6,7|1,4|10]
	 * 	POKEMON_ENTRENADORES=[jolteon,flareon,umbreon,espeon,vaporeon]
		OBJETIVOS_ENTRENADORES=[vaporeon,jolteon,flareon,umbreon,espeon]

		Deberia detectar intercambios:
		*E1 con E5 => Jolteon por Vaporeon
		*E2 con E5 => Flareon por Jolteon
		*E3 con E5 => Ubmreon por Flareon
		*E4 con E5 => Espeon por Umbreon (doble intercambio)
	 */

	resolver_deadlock();
}

void test_deadlock2(){

	/*
	 * Para testear, la mejor forma es cambiando el config. Dejamos el que usamos para
	 * esta prueba:
	 *
	 * 	Prueba inventada.
	 * 	Caso: Que los entrenadores tengan muchos pokemones, y que se
	 * 	priorize el doble intercambio y el mas cercano
	 * 	(en el anterior test no pasaba porque solo habia un intercambio
	 * 	posible en cada caso)
	 *
	 * 	POSICIONES_ENTRENADORES=[1|1,3|3,5|5,7|7,10|10]
	 * 	POKEMON_ENTRENADORES=[bulbasaur|pikachu|gengar,pikachu|charmander|umbreon,charmander|jolteon|pidgey,flareon|onix|vaporeon,squirtle|caterpie|caterpie]
		OBJETIVOS_ENTRENADORES=[charmander|pikachu|squirtle,flareon|jolteon|vaporeon,umbreon|onix|gengar,bulbasaur|charmander|pikachu,pidgey|caterpie|caterpie]

		Entrenador 1: [1|1]
		Adquiridos: [pikachu, gengar, bulbasaur]
		Necesita: 	[pikachu, squirtle, charmander]

		Entrenador 2: [3|3]
		Adquiridos: [umbreon, pikachu, charmander]
		Necesita: 	[vaporeon, jolteon, flareon]

		Entrenador 3: [5|5]
		Adquiridos: [pidgey, jolteon, charmander]
		Necesita: 	[umbreon, onix, gengar]

		Entrenador 4: [7|7]
		Adquiridos: [vaporeon, onix, flareon]
		Necesita: 	[pikachu, charmander, bulbasaur]

		Entrenador 5: [10|10]
		Adquiridos: [squirtle, caterpie, caterpie]
		Necesita: 	[pidgey, caterpie, caterpie]



		Secuencia:
		1. Entrenador 1: Busca squirtle => [5] => dos posibles => da el primero (gengar)
		-E1 con E5, Squirtle por Gengar-

		2. Entrenador 1: Busca charmander => [2,3] => sin dobles, mas cercano? 3 (porque E1 se movio hasta E5) => solo un intercambio posible
		-E1 con E3, Charmander por Bulbasaur-

		3. Entrenador 2: Busca vaporeon => [4] => dos posibles => doble? Si (pikachu)
		-E2 con E4, Vaporeon por Pikachu-

		4. Entrenador 2: Busca jolteon => [3] => doble? Si (umbreon)
		-E2 con E3, Jolteon por Umbreon-

		5. Entrenador 2: Busca flareon => [4] => unico cambio (doble)
		-E2 con E4, Flareon por Charmander-

		6. Entrenador 3: Busca onix => [4] => doble? Si (bulbasaur)
		-E3 con E4, Onix por Bulbasaur-

		7. Entrenador 3: Busca gengar => [5] => unico cambio (doble)
		-E3 con E5, Gengar por Pidgey-



		Deberia detectar intercambios:
		-E1 con E5, Squirtle por Gengar-
		-E1 con E3, Charmander por Bulbasaur-
		-E2 con E4, Vaporeon por Pikachu-
		-E2 con E3, Jolteon por Umbreon-
		-E2 con E4, Flareon por Charmander-
		-E3 con E4, Onix por Bulbasaur-
		-E3 con E5, Gengar por Pidgey-
	 */

	resolver_deadlock();
}
