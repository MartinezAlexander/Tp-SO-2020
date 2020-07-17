#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <stdint.h>

t_config* config;

uint32_t tamano_memoria;
uint32_t tamano_minimo_particion;

char* algoritmo_memoria;
char* algoritmo_reemplazo;
char* algoritmo_particion_libre;

uint32_t frecuencia_compactacion;

char* ip_broker;
char* puerto_broker;
char* path_logger;

void iniciar_config(char* path_config);
void obtener_info_del_config();
void finalizar_config();
#endif
