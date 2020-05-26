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

#endif /* PLANIFICACION_PLANIFICACION_SJF_H_ */
