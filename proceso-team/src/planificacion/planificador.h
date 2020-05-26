/*
 * planificador.h
 *
 *  Created on: 13 may. 2020
 *      Author: utnso
 */

#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<commons/string.h>
#include<commons/collections/queue.h>
#include<semaphore.h>

#include"../variables_globales.h"
#include"../entrenador.h"

typedef enum{
	FIFO, RR, SJF_CD, SJF_SD
}tipo_planificacion;

typedef struct{
	tipo_planificacion algoritmo_planificacion;
	uint32_t quantum;
	uint32_t estimacion_inicial;
	t_queue* cola;
	t_entrenador* entrenadorEnExec;
	sem_t semaforo;
}t_planificador;


t_planificador *planificador;

int puedo_ejecutar();

tipo_planificacion obtener_algoritmo(char* algoritmo);
t_planificador* planificador_create(char* algoritmo, uint32_t quantum, uint32_t estimacion_inicial);

#endif /* PLANIFICADOR_H_ */
