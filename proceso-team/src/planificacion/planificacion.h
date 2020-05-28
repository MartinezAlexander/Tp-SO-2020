#ifndef PLANIFICACION_H_
#define PLANIFICACION_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<mensajes/pokemon.h>
#include<mensajes/posiciones.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#include "../entrenador.h"
#include "../utils/posicion_utils.h"
#include "planificacion_fifo.h"
#include "planificacion_rr.h"
#include "planificacion_sjf.h"
#include "planificador.h"


int entrenador_disponible(t_entrenador *entrenador);

void entrenador_entrar_a_planificacion(t_pokemon* pokemon);

void planificar();
void ejecutar_hilo(t_entrenador* entrenador);

void planificador_iniciar_hilo_entrenador(t_entrenador* entrenador);


#endif /* PLANIFICACION_H_ */
