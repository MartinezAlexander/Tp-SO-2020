/*
 * log_utils.h
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#ifndef UTILS_LOG_UTILS_H_
#define UTILS_LOG_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<commons/log.h>
#include<commons/string.h>
#include<mensajes/posiciones.h>
#include<mensajes/mensajes.h>
#include<mensajes/pokemon.h>
#include "../variables_globales.h"

void loggear_planificacion();
void loggear_movimiento_entrenador(int identificador_entrenador, t_posicion posicion);
void loggear_operacion_atrapar(t_pokemon* pokemon);
void loggear_operacion_intercambio();
void loggear_inicio_deteccion_deadlock();
void loggear_resultado_deteccion_deadlock();
void loggear_nuevo_mensaje(t_mensaje* mensaje);
void loggear_resultado_team();
void loggear_error_broker();
void loggear_inicio_reintento_broker();
void loggear_resultado_reintento_broker();

#endif /* UTILS_LOG_UTILS_H_ */
