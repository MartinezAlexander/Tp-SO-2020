#ifndef ENTRENADOR_H
#define ENTRENADOR_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include <mensajes/posiciones.h>
#include <semaphore.h>
#include <pthread.h>
#include<unistd.h>

//#include "planificacion.h"


uint32_t retardo_cpu;

t_list* entrenadores;


typedef struct{
	char* nombre;
	t_posicion posicion;
} t_pokemon;

typedef enum{
	NEW, READY, EXEC, BLOCKED, EXIT
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





/*
typedef struct{
	char* pokemon;
	uint32_t cantidad;
} t_objetivo;
*/


int mover_proxima_posicion(t_entrenador* entrenador);
void enviar_catch(t_entrenador* entrenador);
void ejecutar_entrenador(t_entrenador* entrenador);

int checkear_exec_entrenador(t_entrenador *entrenador);

t_pokemon* pokemon_create(char* nombre, t_posicion posicion);
t_entrenador* entrenador_create(char* posicion, char* pokemones, char* objetivos);

void entrenador_mostrar(t_entrenador *entrenador);

t_list* array_to_list(char** adquiridos);

t_list* leer_entrenadores(t_config* config);
int cantidad_de_elementos(char** array);



#endif
