
#ifndef PROCESO_BROKER_H_
#define PROCESO_BROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<mensajes/server.h>
#include<mensajes/suscripcion.h>
#include<mensajes/mensajes.h>
#include<mensajes/localized_pokemon.h>
#include<mensajes/appeared_pokemon.h>
#include<mensajes/get_pokemon.h>
#include<mensajes/catch_pokemon.h>
#include<mensajes/caught_pokemon.h>
#include<mensajes/new_pokemon.h>
#include "procesar.h"


t_config* config;

uint32_t tamano_memoria;
uint32_t tamano_minimo_particion;

char* algoritmo_memoria;
char* algoritmo_reemplazo;
char* algoritmo_particion_libre;

uint32_t frecuencia_compactacion;

char* ip_broker;
char* puerto_broker;

t_log* logger;

int32_t ultimo_id;

pthread_t hilo_suscripcion;

t_log* iniciar_logger(char* path);
t_config* leer_config(void);
void inicializar_colas_mensajeria();
void inicializar_variables_globales();
void administrar_mensajes(int* socket);
void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje);

#endif /* PROCESO_BROKER_H_ */
