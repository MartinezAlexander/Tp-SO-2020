#ifndef ENTRENADOR_H
#define ENTRENADOR_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include<mensajes/posiciones.h>
#include<mensajes/pokemon.h>
#include<mensajes/mensajes.h>
#include<mensajes/client.h>
#include<mensajes/catch_pokemon.h>
#include<mensajes/caught_pokemon.h>
#include<semaphore.h>
#include<pthread.h>

#include "variables_globales.h"
#include "utils/array_utils.h"
#include "utils/posicion_utils.h"
#include "utils/log_utils.h"


uint32_t retardo_cpu;

t_list* entrenadores;

//Blocked by catch -> bloqueado a la espera de un mensaje caught, luego de enviar el catch
//Blocked -> bloqueado por respuesta negativa del caught
typedef enum{
	NEW, READY, EXEC, BLOCKED_DEADLOCK, BLOCKED, BLOCKED_BY_CATCH, EXIT
} estado_planificacion;

typedef struct{
	uint32_t ultima_rafaga;
	double ultima_estimacion;
} estado_sjf;

typedef struct{
	int identificador;
	t_posicion posicion;

	t_list* objetivos;
	t_list* pokemones_adquiridos;

	t_pokemon* objetivo_actual;

	estado_planificacion estado;
	estado_sjf* estado_sjf;

	pthread_t hilo;
	sem_t semaforo;
} t_entrenador;


t_dictionary* mensajes_catch_pendientes;
void iniciarlizar_diccionario_catch();

int mover_proxima_posicion(t_entrenador* entrenador);
void enviar_catch(t_entrenador* entrenador);
/*
 * Realiza el proximo ciclo de ejecucion del entrenador,
 * en caso de que haya finalizado su ejecucion devuelve 1,
 * de lo contrario retorna 0
 */
int ejecutar_entrenador(t_entrenador* entrenador);

void resolver_caught_positivo(t_entrenador* entrenador);
void resolver_caught_negativo(t_entrenador* entrenador);

void actualizar_estadistica_entrenador(int id_entrenador);

int entrenador_en_ejecucion(t_entrenador *entrenador);

t_entrenador* entrenador_create(char* posicion, char* pokemones, char* objetivos, int identificador, double estimacion_inicial);
t_list* leer_entrenadores(t_config* config, double estimacion_inicial);

void entrenador_resetear_objetivo(t_entrenador* entrenador);
void entrenador_atrapar_objetivo(t_entrenador* entrenador);

int entrenador_estado_deadlock(t_entrenador* entrenador);
int cumplio_objetivo_entrenador(t_entrenador* entrenador);

t_entrenador* obtener_entrenador_mas_cercano(t_list* entrenadores, t_posicion posicion);


#endif
