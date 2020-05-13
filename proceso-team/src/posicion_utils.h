/*
 * posicion_utils.h
 *
 *  Created on: 13 may. 2020
 *      Author: utnso
 */

#ifndef POSICION_UTILS_H_
#define POSICION_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<mensajes/posiciones.h>
#include "entrenador.h"

int movimientos_entre_posiciones(t_posicion a, t_posicion b);
t_entrenador* obtener_entrenador_mas_cercano(t_list* entrenadores, t_posicion posicion);

#endif /* POSICION_UTILS_H_ */
