/*
 ============================================================================
 Name        : proceso-broker.c
 Author      : Grupo 7 - SO
 Version     : v1.0
 Copyright   : Your copyright notice
 Description : Administrador de cola de mensajes
 ============================================================================
 */

#include "proceso-broker.h"

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

t_cola_mensajeria* cola_mensajeria_new;
t_cola_mensajeria* cola_mensajeria_appeared;
t_cola_mensajeria* cola_mensajeria_get;
t_cola_mensajeria* cola_mensajeria_localized;
t_cola_mensajeria* cola_mensajeria_catch;
t_cola_mensajeria* cola_mensajeria_caught;

uint32_t ultimo_id = 0;

void inicializar_variables_globales(){
	tamano_memoria = config_get_int_value(config, "TAMANO_MEMORIA");
	tamano_minimo_particion = config_get_int_value(config, "TAMANO_MINIMO_PARTICION");

	algoritmo_particion_libre = config_get_string_value(config, "ALGORITMO_PARTICION_LIBRE");
	algoritmo_reemplazo = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
	algoritmo_memoria = config_get_string_value(config, "ALGORITMO_MEMORIA");

	ip_broker = config_get_string_value(config, "IP_BROKER");
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");

	frecuencia_compactacion = config_get_int_value(config, "FRECUENCIA_COMPACTACION");

	char* path_logger = config_get_string_value(config, "LOG_FILE");
	logger = iniciar_logger(path_logger);
}

void inicializar_colas_mensajeria(){
	cola_mensajeria_new = cola_mensajeria_create();
	cola_mensajeria_appeared = cola_mensajeria_create();
	cola_mensajeria_get = cola_mensajeria_create();
	cola_mensajeria_localized = cola_mensajeria_create();
	cola_mensajeria_catch = cola_mensajeria_create();
	cola_mensajeria_caught = cola_mensajeria_create();
}


int main(void) {

	config = leer_config();

	inicializar_variables_globales();

	inicializar_colas_mensajeria();


	return EXIT_SUCCESS;
}



t_log* iniciar_logger(char* path){
	t_log* logger;
	if((logger = log_create(path,"broker",true,LOG_LEVEL_INFO)) == NULL)
	{
		printf("No se pudo crear el log\n");
		exit(1);
	}
	return logger;
}

t_config* leer_config(void){
	t_config *config;//Puede que este mal el path
	if((config = config_create("src/broker.config")) == NULL)
	{
		printf("No pude leer la config\n");
		exit(2);
	}
	return config;
}

