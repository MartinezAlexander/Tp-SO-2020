#include "posicion_utils.h"

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

int movimientos_entre_posiciones(t_posicion a, t_posicion b){
	int distX = abs(a.posicionX - b.posicionX);
	int distY = abs(a.posicionY - b.posicionY);

	return distX + distY;
}

t_entrenador* obtener_entrenador_mas_cercano(t_list* entrenadores, t_posicion posicion)
{
	int index_mas_cercano;
	//Ya empieza como la maxima distancia posible
	int distancia_mas_corta = posicion.posicionX + posicion.posicionY;

	//Comparo la cantidad de movimientos que necesito para llegar a pos.
	for(int i = 0 ; i < list_size(entrenadores) ; i++){
		t_posicion posicion_entrenador = ((t_entrenador*)list_get(entrenadores, i))->posicion;

		int distancia = movimientos_entre_posiciones(posicion, posicion_entrenador);

		//En caso de empate se queda el que ya estaba (el primero)
		if(distancia < distancia_mas_corta){
			distancia_mas_corta = distancia;
			index_mas_cercano = i;
		}
	}

	return list_get(entrenadores, index_mas_cercano);
}
