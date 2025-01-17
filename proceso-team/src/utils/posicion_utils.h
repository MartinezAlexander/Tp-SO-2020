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
#include<commons/collections/list.h>
#include "../entrenador.h"

int movimientos_entre_posiciones(t_posicion a, t_posicion b);
int direccion_en_x(t_posicion desde, t_posicion hasta);
int direccion_en_y(t_posicion desde, t_posicion hasta);

#endif /* POSICION_UTILS_H_ */
