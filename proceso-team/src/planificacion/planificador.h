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

typedef enum{
	FIFO, RR, SJF_CD, SJF_SD
}tipo_planificacion;

typedef struct{
	tipo_planificacion algoritmo_planificacion;
	uint32_t quantum;
	uint32_t estimacion_inicial;
	t_queue* cola;
}t_planificador;


t_planificador *planificador;

tipo_planificacion obtener_algoritmo(char* algoritmo);
t_planificador* planificador_create(char* algoritmo, uint32_t quantum, uint32_t estimacion_inicial);

#endif /* PLANIFICADOR_H_ */
