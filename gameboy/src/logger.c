/*
 * logger.c
 *
 *  Created on: 7 jun. 2020
 *      Author: utnso
 */

#include "logger.h"

void iniciar_logger(char* path) {

	if ((logger = log_create(path, "gameboy", true, LOG_LEVEL_INFO)) == NULL) {
		printf("No se pudo crear el log\n");
		exit(1);
	}
}

void loggear_info(char* info) {
	log_info(logger, info);
}

void loggear_conexion(t_proceso id_proceso,int conexion) {
	if(conexion > 0){
	char* log = string_from_format("El gameboy se conecto al proceso %s", proceso_to_string(id_proceso));
	log_info(logger, log);
	}
}


void loggear_desconexion(t_proceso id_proceso){
	char* log = string_from_format("El gameboy se desconecto del proceso %s", proceso_to_string(id_proceso));
	log_info(logger,log);
}

void loggear_suscripcion(op_code id_cola) {

	char* log = string_from_format("El proceso se suscribe a la cola %s",op_code_to_string(id_cola));
	log_info(logger, log);
}

void loggear_nuevo_mensaje(op_code id_cola,char* mensaje) {
	char* log = string_from_format("Llego un nuevo mensaje: %s a la cola %s",mensaje,op_code_to_string(id_cola));
	log_info(logger, log);
}

char* proceso_to_string(t_proceso id_proceso){
	char* broker = "BROKER";
	char* team = "TEAM";
	char* gamecard = "GAMECARD";
	char* gameboy = "GAMEBOY";
	char* suscriptor = "SUSCRIPTOR";
	char* proceso;

	switch(id_proceso){
	case BROKER:
		proceso= broker;
		break;
	case TEAM:
		proceso = team;
		break;
	case GAMECARD:
		proceso = gamecard;
		break;
	case GAMEBOY:
		proceso = gameboy;
		break;
	case SUSCRIPTOR:
		proceso = suscriptor;
		break;
	}
	return proceso;
}
