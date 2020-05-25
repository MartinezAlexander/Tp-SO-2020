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

	ultimo_id = 0;
}

void inicializar_colas_mensajeria(){
	cola_mensajeria_new = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_new->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_new);
	cola_mensajeria_appeared = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_appeared->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_appeared);
	cola_mensajeria_get = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_get->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_get);
	cola_mensajeria_localized = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_localized->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_localized);
	cola_mensajeria_catch = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_catch->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_catch);
	cola_mensajeria_caught = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_caught->hilo),NULL,(void*)procesar_pokemon,cola_mensajeria_caught);
}

int main(void){
	config = leer_config();

	inicializar_variables_globales();

	inicializar_colas_mensajeria();

	iniciar_servidor(ip_broker,puerto_broker,administrar_mensajes);

	return EXIT_SUCCESS;
}

void administrar_mensajes(int* socket){
	t_mensaje* mensaje;
	mensaje = recibir_mensaje(*socket);
	switch(mensaje->codigo){
		case SUSCRIPCION:
			procesar_suscripcion(mensaje, socket);
			break;
		case NEW_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_new,mensaje,&ultimo_id);
			enviar_confirmacion(ultimo_id, ID,*socket);
			break;
		case LOCALIZED_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_localized,mensaje,&ultimo_id);
			enviar_confirmacion(ultimo_id, ID,*socket);
			break;
		case GET_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_get,mensaje,&ultimo_id);
			enviar_confirmacion(ultimo_id, ID,*socket);
			break;
		case APPEARED_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_appeared,mensaje,&ultimo_id);
			enviar_confirmacion(ultimo_id, ID,*socket);
			break;
		case CATCH_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_catch,mensaje,&ultimo_id);
			enviar_confirmacion(ultimo_id, ID,*socket);
			break;
		case CAUGHT_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_caught,mensaje,&ultimo_id);
			enviar_confirmacion(ultimo_id, ID,*socket);
			break;
		default:
			printf("CODIGO DE MENSAJE NO VALIDO \n\n");
	}
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
	if((config = config_create("/home/utnso/workspace/tp-2020-1c-Grupo-7-SO/proceso-broker/src/broker.config")) == NULL)
	{
		printf("No pude leer la config\n");
		exit(2);
	}
	return config;
}
