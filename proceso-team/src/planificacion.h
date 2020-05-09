#ifndef PLANIFICACION_H_
#define PLANIFICACION_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<mensajes/posiciones.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#include "entrenador.h"

typedef struct{
	char* algoritmo_planificacion;
	uint32_t quantum;
	uint32_t estimacion_inicial;
	t_queue cola;
}t_planificador;

t_planificador* planificador_create(char* algoritmo, uint32_t quantum, uint32_t estimacion_inicial);

#endif /* PLANIFICACION_H_ */
