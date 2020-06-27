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

struct t_entrenador;
typedef struct unEntrenador t_entrenador;

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
	//En caso de que este usando planificacion con desalojo, el debo_desalojar
	//indica si al finalizar la ejecucion del ciclo actual, me deben desalojar
	//y encolar devuelta
	int debo_desalojar_al_fin_de_ciclo;
	pthread_mutex_t mutex_desalojo;
	//Y los dos semaforos me ayudan a sincronizar el encolamiento en el desalojo
	//ya que lo que hacemos cuando llega alguien nuevo para planificar es:
	//esperarar a que el actual ejecute -> encolar actual -> encolar nuevo -> replanificar
	//Entonces uso estos semaforos para eso
	sem_t semaforo_desalojo;
	sem_t semaforo_post_desalojo;

	//sem_t semaforo;
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
