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
#include "mensajes/procesamiento_appeared.h"

#include "deadlock.h"

struct t_intercambio;
typedef struct unIntercambio t_intercambio;

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
	int empezo_a_ejecutar;
	//Representa el hecho de si un entrenador empezo a ejecutar y no termino todavia
	//ya sea porque debo replanificar en el medio, o porque fue desalojado
} estado_sjf;

typedef struct unEntrenador{
	int identificador;
	t_posicion posicion;

	t_list* objetivos;
	t_list* pokemones_adquiridos;

	t_pokemon* objetivo_actual;
	t_intercambio* intercambio_actual;

	estado_planificacion estado;
	estado_sjf* estado_sjf;

	pthread_t hilo; //Hilo de ejecucion
	sem_t semaforo; //Para controlar su hilo de ejecucion
	pthread_mutex_t mutex_lectura_entrenador;
} t_entrenador;

//Aca se guardan todos los ids de catch que enviamos junto con el entrenador
//que lo envio, asi cuando me llega un caught puedo realizar la operacion
//para el mensaje y entrenador correspondiente
t_dictionary* mensajes_catch_pendientes;


void iniciarlizar_diccionario_catch();

int mover_proxima_posicion(t_entrenador* entrenador, t_posicion objetivo);
void enviar_catch(t_entrenador* entrenador);
/*
 * Realiza el proximo ciclo de ejecucion del entrenador,
 * en caso de que haya finalizado su ejecucion devuelve 1,
 * de lo contrario retorna 0
 */
int ejecutar_entrenador(t_entrenador* entrenador);
int ejecutar_entrenador_intercambio_deadlock(t_entrenador* entrenador);

void resolver_caught_positivo(t_entrenador* entrenador, int asincronico);
void resolver_caught_negativo(t_entrenador* entrenador);

void bloquear_entrenador(t_entrenador* entrenador);

void agregar_a_objetivos_globales(char* especie);
void sacar_de_objetivos_globales(char* especie, t_list* objetivos);

void actualizar_estadistica_entrenador(int id_entrenador, int ciclos_a_agregar);

int entrenador_en_ejecucion(t_entrenador *entrenador);
int entrenador_en_deadlock(t_entrenador *entrenador);
int entrenador_termino_ejecucion_normal(t_entrenador *entrenador);

void asignar_pokemones(t_entrenador* entrenador, char* pokemones);
t_entrenador* entrenador_create(char* posicion, char* objetivos, char* adquiridos, int identificador, double estimacion_inicial);
t_list* leer_entrenadores(t_config* config, double estimacion_inicial);

int entrenador_tiene_objetivo(t_entrenador* entrenador, char* especie);

void entrenador_resetear_objetivo(t_entrenador* entrenador);
void entrenador_atrapar_objetivo(t_entrenador* entrenador);

int entrenador_estado_deadlock(t_entrenador* entrenador);
int cumplio_objetivo_entrenador(t_entrenador* entrenador);

t_entrenador* obtener_entrenador_mas_cercano(t_list* entrenadores, t_posicion posicion);
void actualizar_estado_entrenador(t_entrenador* entrenador);


void entrenador_destroy(t_entrenador* e);
void entrenadores_destroy();

#endif
