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

void planificar_sjf_sd();
void planificar_sjf_cd();

t_entrenador* shortest_job();
t_entrenador* shortest_job_con_desalojo();

double calcular_estimacion_con_desalojo(t_entrenador* entrenador);
double calcular_estimacion(t_entrenador* entrenador);


#endif /* PLANIFICACION_PLANIFICACION_SJF_H_ */
