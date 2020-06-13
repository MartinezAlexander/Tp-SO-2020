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

int main(void){
	iniciar_config();

	obtener_info_del_config();

	iniciar_logger(path_logger);

	iniciar_log_personal("../src/broker_personal.log");

	inicializar_ids_mensajes();

	memoria_cache_create();

	if(string_equals_ignore_case(algoritmo_memoria,"BS")){
		//iniciar_buddy_system();
	}else{
		iniciar_administrador_pd();
	}

	inicializar_colas_mensajeria(procesar_pokemon);

	iniciar_servidor(ip_broker,puerto_broker,administrar_mensajes);

	return EXIT_SUCCESS;
}
