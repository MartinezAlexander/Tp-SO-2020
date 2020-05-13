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
#include "posicion_utils.h"
#include "planificacion_fifo.h"



typedef enum{
	FIFO, RR, SJF_CD, SJF_SD
}tipo_planificacion;

typedef struct{
	tipo_planificacion algoritmo_planificacion;
	uint32_t quantum;
	uint32_t estimacion_inicial;
	t_queue* cola;
}t_planificador;


tipo_planificacion obtener_algoritmo(char* algoritmo);
t_planificador* planificador_create(char* algoritmo, uint32_t quantum, uint32_t estimacion_inicial);

int entrenador_disponible(t_entrenador *entrenador);
void encolar(t_planificador* planificador, t_entrenador* entrenador);
int puedo_ejecutar();

void entrenador_entrar_a_planificacion(t_planificador* planificador, t_pokemon* pokemon);

void ejecutar_hilo_fifo(t_entrenador* entrenador);
void ejecutar_proximo(t_planificador* planificador);

void planificador_iniciar_hilo_entrenador(t_planificador* planificador, t_entrenador* entrenador);


#endif /* PLANIFICACION_H_ */
