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
}

void inicializar_colas_mensajeria(){
	cola_mensajeria_new = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_new->hilo),NULL,(void*)procesar_new_pokemon,NULL);
	cola_mensajeria_appeared = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_appeared->hilo),NULL,(void*)procesar_appeared_pokemon,NULL);
	cola_mensajeria_get = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_get->hilo),NULL,(void*)procesar_get_pokemon,NULL);
	cola_mensajeria_localized = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_localized->hilo),NULL,(void*)procesar_localized_pokemon,NULL);
	cola_mensajeria_catch = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_catch->hilo),NULL,(void*)procesar_catch_pokemon,NULL);
	cola_mensajeria_caught = cola_mensajeria_create();
	pthread_create(&(cola_mensajeria_caught->hilo),NULL,(void*)procesar_caught_pokemon,NULL);
}

pthread_t prueba;

void mostrar(){
	 if(list_is_empty(cola_mensajeria_new->suscriptores)){
		 puts("esta vacia");
	 }else{
		 puts("tiene algo");
	 }
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
			queue_push(cola_mensajeria_new->queue,mensaje->mensaje);
			break;
		case LOCALIZED_POKEMON:
			queue_push(cola_mensajeria_localized->queue,mensaje->mensaje);
			break;
		case GET_POKEMON:
			queue_push(cola_mensajeria_get->queue,mensaje->mensaje);
			break;
		case APPEARED_POKEMON:
			queue_push(cola_mensajeria_appeared->queue,mensaje->mensaje);
			break;
		case CATCH_POKEMON:
			queue_push(cola_mensajeria_catch->queue,mensaje->mensaje);
			break;
		case CAUGHT_POKEMON:
			queue_push(cola_mensajeria_caught->queue,mensaje->mensaje);
			break;
		default:
			printf("CODIGO DE MENSAJE NO VALIDO \n\n");
	}
	//pthread_create(&prueba,NULL,(void*)mostrar,NULL);
}

void procesar_suscripcion(t_mensaje* mensaje, int* socket){
	t_suscripcion* suscripcion = (t_suscripcion*)mensaje->mensaje;
	t_suscriptor* suscriptor = suscriptor_create(*socket,suscripcion->pid);
	switch(suscripcion->cola_suscripcion){
		case NEW_POKEMON:
			list_add(cola_mensajeria_new->suscriptores, suscriptor);
			break;
		case LOCALIZED_POKEMON:
			list_add(cola_mensajeria_localized->suscriptores, suscriptor);
			break;
		case GET_POKEMON:
			list_add(cola_mensajeria_get->suscriptores, suscriptor);
			break;
		case APPEARED_POKEMON:
			list_add(cola_mensajeria_appeared->suscriptores, suscriptor);
			break;
		case CATCH_POKEMON:
			list_add(cola_mensajeria_catch->suscriptores, suscriptor);
			break;
		case CAUGHT_POKEMON:
			list_add(cola_mensajeria_caught->suscriptores, suscriptor);
			break;
		default:
			printf("SUSCRIPTOR NO VALIDO \n\n");
	}
	puts("SUSCRIBI A: ");
	suscripcion_proceso_mostrar(suscripcion);
}

void procesar_new_pokemon(){
	while(1){
		if(!queue_is_empty(cola_mensajeria_new->queue)){
			t_new_pokemon* pokemon = (t_new_pokemon*)queue_pop(cola_mensajeria_new->queue);
			printf("saco de la cola a: \n");
			new_pokemon_mostrar(pokemon);
			sleep(2);
		}
	}
}

void procesar_localized_pokemon(t_mensaje* mensaje){
	while(1){
		if(!queue_is_empty(cola_mensajeria_localized->queue)){
			t_localized_pokemon* pokemon = (t_localized_pokemon*)queue_pop(cola_mensajeria_localized->queue);
			printf("saco de la cola a: \n");
			localized_pokemon_mostrar(pokemon);
			sleep(2);
		}
	}
}
void procesar_get_pokemon(t_mensaje* mensaje){
	while(1){
		if(!queue_is_empty(cola_mensajeria_get->queue)){
			t_get_pokemon* pokemon = (t_get_pokemon*)queue_pop(cola_mensajeria_get->queue);
			printf("saco de la cola a: \n");
			get_pokemon_mostrar(pokemon);
			sleep(2);
		}
	}
}
void procesar_appeared_pokemon(t_mensaje* mensaje){
	while(1){
		if(!queue_is_empty(cola_mensajeria_appeared->queue)){
			t_appeared_pokemon* pokemon = (t_appeared_pokemon*)queue_pop(cola_mensajeria_appeared->queue);
			printf("saco de la cola a: \n");
			appeared_pokemon_mostrar(pokemon);
			sleep(2);
		}
	}
}
void procesar_catch_pokemon(t_mensaje* mensaje){
	while(1){
		if(!queue_is_empty(cola_mensajeria_catch->queue)){
			t_catch_pokemon* pokemon = (t_catch_pokemon*)queue_pop(cola_mensajeria_catch->queue);
			printf("saco de la cola a: \n");
			catch_pokemon_mostrar(pokemon);
			sleep(2);
		}
	}
}
void procesar_caught_pokemon(t_mensaje* mensaje){
	while(1){
		if(!queue_is_empty(cola_mensajeria_caught->queue)){
			t_caught_pokemon* pokemon = (t_caught_pokemon*)queue_pop(cola_mensajeria_caught->queue);
			printf("saco de la cola a: \n");
			caught_pokemon_mostrar(pokemon);
			sleep(2);
		}
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
