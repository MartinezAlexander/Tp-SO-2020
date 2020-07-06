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

void controlador_de_seniales(int num_senial){
	if(num_senial == SIGUSR1){
		dump();
	}
	if(num_senial == SIGINT){
		finalizar_programa();
	}
}

int main(void){

	signal(SIGUSR1, controlador_de_seniales);
	signal(SIGINT, controlador_de_seniales);

	iniciar_config();

	obtener_info_del_config();

	iniciar_logger(path_logger);

	iniciar_log_personal("../src/broker_personal.log");

	inicializar_ids_mensajes();

	memoria_cache_create();

	if(string_equals_ignore_case(algoritmo_memoria,"BS")){
		iniciar_buddy_system();
	}else{
		iniciar_administrador_pd();
	}

	iniciar_procesador_suscripciones(procesar_suscripcion);

	inicializar_colas_mensajeria(procesar_pokemon);

	iniciar_servidor(ip_broker,puerto_broker,administrar_mensajes);

	return EXIT_SUCCESS;
}

void finalizar_programa(){
	//procesador_suscripciones_liberar();
	//colas_mensajeria_liberar();
	finalizar_hilos();
	exit(0);
}

void finalizar_hilos(){

	pthread_cancel(cola_mensajeria_new->hilo);
	pthread_cancel(cola_mensajeria_appeared->hilo);
	pthread_cancel(cola_mensajeria_caught->hilo);
	pthread_cancel(cola_mensajeria_catch->hilo);
	pthread_cancel(cola_mensajeria_get->hilo);
	pthread_cancel(cola_mensajeria_localized->hilo);
	pthread_cancel(procesador_suscripciones);

	pthread_detach(procesador_suscripciones);
	pthread_detach(cola_mensajeria_new->hilo);
	pthread_detach(cola_mensajeria_appeared->hilo);
	pthread_detach(cola_mensajeria_caught->hilo);
	pthread_detach(cola_mensajeria_catch->hilo);
	pthread_detach(cola_mensajeria_get->hilo);
	pthread_detach(cola_mensajeria_localized->hilo);
}
