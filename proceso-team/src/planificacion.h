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
/*
char* PLANIFICACION_FIFO = "FIFO";
char* PLANIFICACION_RR = "RR";
char* PLANIFICACION_SJF_S = "SJF_S";
char* PLANIFICACION_SJF_C = "SJF_C";
*/



typedef struct{
	char* algoritmo_planificacion;
	uint32_t quantum;
	uint32_t estimacion_inicial;
	t_queue* cola;
}t_planificador;



t_planificador *planificador;


t_planificador* planificador_create(char* algoritmo, uint32_t quantum, uint32_t estimacion_inicial);


void comenzar_ciclo_planificador(t_entrenador* entrenador);
void finalizar_ciclo_planificador(t_entrenador* entrenador);
void ejecutar_proximo();

int entrenador_disponible(t_entrenador *entrenador);
void encolar(t_entrenador* entrenador);
int puedo_ejecutar();
t_entrenador* obtener_entrenador_mas_cercano(t_list* entrenadores, t_pokemon* pokemon);

void entrenador_entrar_a_planificacion(t_pokemon* pokemon);

void ejecutar_hilo_planificado(t_entrenador* entrenador);

void planificador_iniciar_hilo_entrenador(t_entrenador* entrenador);

int movimientos_entre_posiciones(t_posicion a, t_posicion b);

#endif /* PLANIFICACION_H_ */
