/*
 * logger.c
 *
 *  Created on: 4 jul. 2020
 *      Author: utnso
 */

#include "logger.h"

void iniciar_logger(char* path) {

	if ((logger = log_create(path, "proceso-game-card", true, LOG_LEVEL_INFO)) == NULL) {
		printf("No se pudo crear el log\n");
		exit(1);
	}
}

void loggear_info(char* info) {
	log_info(logger, info);
}

void loggear_error_conexion(int socket) {
	if(socket<=0){
		char* log = string_from_format("El gamecard no se pudo conectar al proceso Broker");
		log_info(logger, log);
		free(log);
	}
}

void loggear_no_existe_posicion(t_posicion posicion){
	char* log = string_from_format("No existe la poscion dada x:%d, y:%d",posicion.posicionX, posicion.posicionY );
	log_info(logger,log);
	free(log);
}
void loggear_no_existe_pokemon(char* especie){
	char*log=string_from_format("No existe el pokemon %s en el archivo",especie);
	log_info(logger,log);
	free(log);
}

void loggear_archivo_abierto(char* especie){
	char* log = string_from_format("El archivo de %s esta abierto",especie);
	log_info(logger,log);
	free(log);
}
void loggear_reintento(char* especie){
	char* log = string_from_format("Reintento abrir el archivo de: %s",especie);
		log_info(logger,log);
		free(log);
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
