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
			ultimo_id++;
			mensaje->id = ultimo_id;//validar id = -1
			queue_push(cola_mensajeria_new->queue,mensaje);
			break;
		case LOCALIZED_POKEMON:
			ultimo_id++;
			mensaje->id = ultimo_id;//validar id = -1
			queue_push(cola_mensajeria_localized->queue,mensaje);
			break;
		case GET_POKEMON:
			ultimo_id++;
			mensaje->id = ultimo_id;//validar id = -1
			queue_push(cola_mensajeria_get->queue,mensaje);
			break;
		case APPEARED_POKEMON:
			ultimo_id++;
			mensaje->id = ultimo_id;//validar id = -1
			queue_push(cola_mensajeria_appeared->queue,mensaje);
			break;
		case CATCH_POKEMON:
			ultimo_id++;
			mensaje->id = ultimo_id;//validar id = -1
			queue_push(cola_mensajeria_catch->queue,mensaje);
			break;
		case CAUGHT_POKEMON:
			ultimo_id++;
			mensaje->id = ultimo_id; //validar id = -1
			queue_push(cola_mensajeria_caught->queue,mensaje);
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
			//mandar mensajes viejos de la cache
			break;
		case LOCALIZED_POKEMON:
			list_add(cola_mensajeria_localized->suscriptores, suscriptor);
			//mandar mensajes viejos de la cache
			break;
		case GET_POKEMON:
			list_add(cola_mensajeria_get->suscriptores, suscriptor);
			//mandar mensajes viejos de la cache
			break;
		case APPEARED_POKEMON:
			list_add(cola_mensajeria_appeared->suscriptores, suscriptor);
			//mandar mensajes viejos de la cache
			break;
		case CATCH_POKEMON:
			list_add(cola_mensajeria_catch->suscriptores, suscriptor);
			//mandar mensajes viejos de la cache
			break;
		case CAUGHT_POKEMON:
			list_add(cola_mensajeria_caught->suscriptores, suscriptor);
			//mandar mensajes viejos de la cache
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
			if(!list_is_empty(cola_mensajeria_new->suscriptores)){
				t_mensaje* pokemon = (t_mensaje*)queue_pop(cola_mensajeria_new->queue);
				//guardar pokemon en la cache
				printf("saco de la cola a: \n");
				envio_a_suscriptores(cola_mensajeria_new->suscriptores,pokemon);
				new_pokemon_mostrar((t_new_pokemon*)pokemon->mensaje);
			}
		}
	}
}

void procesar_localized_pokemon(t_mensaje* mensaje){
	while(1){
		if(!queue_is_empty(cola_mensajeria_localized->queue)){
			if(!list_is_empty(cola_mensajeria_localized->suscriptores)){
				t_mensaje* pokemon = (t_mensaje*)queue_pop(cola_mensajeria_localized->queue);
				//guardar pokemon en la cache
				printf("saco de la cola a: \n");
				envio_a_suscriptores(cola_mensajeria_localized->suscriptores,pokemon);
				localized_pokemon_mostrar((t_localized_pokemon*)pokemon->mensaje);
			}
		}
	}
}
void procesar_get_pokemon(t_mensaje* mensaje){
	while(1){
		if(!queue_is_empty(cola_mensajeria_get->queue)){
			if(!list_is_empty(cola_mensajeria_get->suscriptores)){
				t_mensaje* pokemon = (t_mensaje*)queue_pop(cola_mensajeria_get->queue);
				//guardar pokemon en la cache
				printf("saco de la cola a: \n");
				envio_a_suscriptores(cola_mensajeria_get->suscriptores,pokemon);
				get_pokemon_mostrar((t_get_pokemon*)pokemon->mensaje);
			}
		}
	}
}
void procesar_appeared_pokemon(t_mensaje* mensaje){
	while(1){
		if(!queue_is_empty(cola_mensajeria_appeared->queue)){
			if(!list_is_empty(cola_mensajeria_appeared->suscriptores)){
				t_mensaje* pokemon = (t_mensaje*)queue_pop(cola_mensajeria_appeared->queue);
				//guardar pokemon en la cache
				printf("saco de la cola a: \n");
				envio_a_suscriptores(cola_mensajeria_appeared->suscriptores,pokemon);
				appeared_pokemon_mostrar((t_appeared_pokemon*)pokemon->mensaje);
			}
		}
	}
}
void procesar_catch_pokemon(t_mensaje* mensaje){
	while(1){
		if(!queue_is_empty(cola_mensajeria_catch->queue)){
			if(!list_is_empty(cola_mensajeria_catch->suscriptores)){
				t_mensaje* pokemon = (t_mensaje*)queue_pop(cola_mensajeria_catch->queue);
				//guardar pokemon en la cache
				printf("saco de la cola a: \n");
				envio_a_suscriptores(cola_mensajeria_catch->suscriptores,pokemon);
				catch_pokemon_mostrar((t_catch_pokemon*)pokemon->mensaje);
			}
		}
	}
}
void procesar_caught_pokemon(t_mensaje* mensaje){
	while(1){
		if(!queue_is_empty(cola_mensajeria_caught->queue)){
			if(!list_is_empty(cola_mensajeria_caught->suscriptores)){
				t_mensaje* pokemon = (t_mensaje*)queue_pop(cola_mensajeria_caught->queue);
				//guardar pokemon en la cache
				printf("saco de la cola a: \n");
				envio_a_suscriptores(cola_mensajeria_caught->suscriptores,pokemon);
				caught_pokemon_mostrar((t_caught_pokemon*)pokemon->mensaje);
			}
		}
	}
}

void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje){
	for(int i = 0; i < list_size(suscriptores); i++){
		t_suscriptor* suscriptor = list_get(suscriptores,i);
		enviar_mensaje(mensaje,suscriptor->socket);
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
