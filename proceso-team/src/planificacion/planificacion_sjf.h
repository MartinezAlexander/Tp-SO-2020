/*
 * planificacion_sjf.h
 *
 *  Created on: 24 may. 2020
 *      Author: utnso
 */

#ifndef PLANIFICACION_PLANIFICACION_SJF_H_
#define PLANIFICACION_PLANIFICACION_SJF_H_

#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<commons/collections/queue.h>
#include "../entrenador.h"
#include "planificador.h"

void comenzar_ciclo_sjf_sin_desalojo(t_entrenador* entrenador);
void finalizar_ciclo_sjf_sin_desalojo(t_entrenador* entrenador);
void ejecutar_proximo_sjf_sin_desalojo();

void comenzar_ciclo_sjf_con_desalojo(t_entrenador* entrenador);
void finalizar_ciclo_sjf_con_desalojo(t_entrenador* entrenador);
void ejecutar_proximo_sjf_con_desalojo();

#endif /* PLANIFICACION_PLANIFICACION_SJF_H_ */
