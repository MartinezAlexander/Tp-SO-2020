#include "direccion_utils.h"


/**
 * Devuelve:
 *  -1 si la posicion 'hasta' se encuentra a la izquierda de la posicion 'desde',
 *  1 si la posicion 'hasta' se encientra a la derecha de la posicion 'desde',
 *  0 si las posiciones en x coinciden
 */
int direccion_en_x(t_posicion desde, t_posicion hasta){
	int distancia = hasta.posicionX - desde.posicionX;
	if(distancia == 0) return 0;

	int modulo_distancia = abs(distancia);

	return (distancia/modulo_distancia);
}

/**
 * Devuelve:
 *  -1 si la posicion 'hasta' se encuentra a la arriba de la posicion 'desde',
 *  1 si la posicion 'hasta' se encientra a la abajo de la posicion 'desde',
 *  0 si las posiciones en y coinciden
 */
int direccion_en_y(t_posicion desde, t_posicion hasta){
	int distancia = hasta.posicionY - desde.posicionY;
	if(distancia == 0) return 0;

	int modulo_distancia = abs(distancia);

	return (distancia/modulo_distancia);
}
