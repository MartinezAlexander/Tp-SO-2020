#ifndef ENTRENADOR_H
#define ENTRENADOR_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include<mensajes/posiciones.h>
#include<mensajes/pokemon.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<mensajes/mensajes.h>
#include<mensajes/client.h>
#include<mensajes/catch_pokemon.h>
#include<mensajes/caught_pokemon.h>

#include "array_utils.h"
#include "variables_globales.h"
#include "objetivos.h"


uint32_t retardo_cpu;

t_list* entrenadores;

//Blocked by catch -> bloqueado a la espera de un mensaje caught, luego de enviar el catch
//Blocked -> bloqueado por respuesta negativa del caught
typedef enum{
	NEW, READY, EXEC, BLOCKED_DEADLOCK, BLOCKED, BLOCKED_BY_CATCH, EXIT
} estado_planificacion;

typedef struct{
	t_posicion posicion;
	estado_planificacion estado;
	t_list* objetivos;
	t_list* pokemones_adquiridos;

	pthread_t hilo;
	sem_t semaforo;

	t_pokemon* objetivo_actual;
} t_entrenador;


t_dictionary* mensajes_catch_pendientes;
void iniciarlizar_diccionario_catch();

int mover_proxima_posicion(t_entrenador* entrenador);
void enviar_catch(t_entrenador* entrenador);
void ejecutar_entrenador(t_entrenador* entrenador);

int entrenador_en_ejecucion(t_entrenador *entrenador);

t_entrenador* entrenador_create(char* posicion, char* pokemones, char* objetivos);
t_list* leer_entrenadores(t_config* config);
void entrenador_mostrar(t_entrenador *entrenador);

void entrenador_resetear_objetivo(t_entrenador* entrenador);
void entrenador_atrapar_objetivo(t_entrenador* entrenador);

int entrenador_estado_deadlock(t_entrenador* entrenador);
int cumplio_objetivo_entrenador(t_entrenador* entrenador);


#endif
