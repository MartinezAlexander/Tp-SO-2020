#include "test_deadlock.h"

void test_deadlock(){

	/*
	 * Para testear, la mejor forma es cambiando el config. Dejamos el que usamos para
	 * esta prueba:
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
