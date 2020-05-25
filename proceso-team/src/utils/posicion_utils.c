#include "posicion_utils.h"


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
