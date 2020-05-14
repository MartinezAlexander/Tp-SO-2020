/*
 * planificacion_fifo.h
 *
 *  Created on: 13 may. 2020
 *      Author: utnso
 */

#ifndef PLANIFICACION_FIFO_H_
#define PLANIFICACION_FIFO_H_


#include<semaphore.h>
#include<commons/collections/queue.h>
#include "entrenador.h"
#include "planificador.h"

void comenzar_ciclo_fifo(t_entrenador* entrenador);
void finalizar_ciclo_fifo(t_entrenador* entrenador);
void ejecutar_proximo_fifo();

#endif /* PLANIFICACION_FIFO_H_ */
