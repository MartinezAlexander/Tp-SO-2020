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
#include<pthread.h>
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
	t_queue* cola;

	uint32_t quantum;
	uint32_t quantum_actual;

	uint32_t estimacion_inicial;
	double alpha;

	t_entrenador* entrenador_en_exec;

	sem_t semaforo;
	pthread_t hilo_planificacion;

	pthread_mutex_t mutex_planificacion;
}t_planificador;


t_planificador *planificador;

int hay_alguien_en_ejecucion();
void entrar_a_ejecucion(t_entrenador* entrenador);

/**
 * Saca el entrenador actual de ejecucion (osea, limpia la variable entrenador en exec)
 */
void sacar_de_ejecucion();

void encolar(t_entrenador* entrenador);

tipo_planificacion obtener_algoritmo(char* algoritmo);
t_planificador* planificador_create(char* algoritmo, uint32_t quantum, uint32_t estimacion_inicial, double alpha);

#endif /* PLANIFICADOR_H_ */
