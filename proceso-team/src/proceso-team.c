/*
 ============================================================================
 Name        : proceso-team.c
 Author      : Grupo 7 - SO
 Version     : v1.0
 Copyright   : Your copyright notice
 Description : Administrar distintos entrenadores “planificándolos” dentro de
			   un mapa de dos coordenadas.
 ============================================================================
 */

#include "proceso-team.h"

//Variables de un proceso team
t_config* config;

t_list* entrenadores;
t_list* objetivo_global;

uint32_t tiempo_de_reconexion;
uint32_t retardo_cpu;

//TODO estructura para algoritmos
char* algoritmo_planificacion;
uint32_t quantum;
uint32_t estimacion_inicial;

char* ip_broker;
char* puerto_broker;

t_log* logger;


int main(void) {

	//Leer archivo config y settear variables
	config = leer_config();

	entrenadores = leer_entrenadores(config);
	objetivo_global = obtener_objetivo_global(entrenadores);

	tiempo_de_reconexion = config_get_int_value(config, "TIEMPO_RECONEXION");
	retardo_cpu = config_get_int_value(config, "RETARDO_CICLO_CPU");

	algoritmo_planificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	quantum = config_get_int_value(config, "QUANTUM");
	estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");

	ip_broker = config_get_string_value(config, "IP_BROKER");
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");

	char* path_logger = config_get_string_value(config, "LOG_FILE");
	logger = iniciar_logger(path_logger);

	//Conectarse a las colas del broker
	iniciar_conexion_broker(ip_broker, puerto_broker);

	//Enviar mensaje GET al broker segun objetivos globales
	enviar_get_objetivo(objetivo_global);

	//Abrir socket de escucha para el Gameboy
	iniciar_puerto_de_escucha();

	terminar_programa(logger, config);
}

void iniciar_conexion_broker(char* ip, char* puerto){

}

void iniciar_puerto_de_escucha(){
	iniciar_servidor("127.0.0.2", "5002",(void*) procesar_mensajes);
}

void procesar_mensajes(int* socket){
	op_code operacion;
	void* mensaje = recibir_mensaje(*socket, &operacion);

	switch(operacion){
		case LOCALIZED_POKEMON:
			procesar_localized((t_localized_pokemon*) mensaje);
			break;
		case APPEARED_POKEMON:
			procesar_appeared((t_appeared_pokemon*) mensaje);
			break;
		case CAUGHT_POKEMON:
			procesar_caught((t_caught_pokemon*) mensaje);
			break;
		default:
			printf("Mensaje no correspondiente al proceso team (op: %d) \n", operacion);
			break;
	}
}

void procesar_localized(t_localized_pokemon* localized_pokemon){

}

void procesar_appeared(t_appeared_pokemon* appeared_pokemon){
	printf("Procesando mensaje APPEARED\n");
	printf("Recibo %s\n\n", appeared_pokemon->nombre);

	if(cumplio_objetivo_global(objetivo_global, entrenadores)){
		//free(appeared_pokemon);
	}else if(pokemon_dentro_de_objetivos(objetivo_global, appeared_pokemon->nombre)){
		//Verifico si ya estoy procesando un localized de la misma especie
		//Planifico entrenador
	}else{
		//free(appeared_pokemon);
	}
}

void procesar_caught(t_caught_pokemon* caught_pokemon){

}


void enviar_get_objetivo(t_list* objetivo_global){

	char* ultima_especie_enviada = "";
	for(int i = 0 ; i < list_size(objetivo_global) ; i++){
		char* pokemon = list_get(objetivo_global, i);

		if(strcmp(pokemon, ultima_especie_enviada) != 0){
			//TODO: Envio mensaje get al broker
			printf("Enviando especie GET al broker: %s\n", pokemon);
			//
			ultima_especie_enviada = pokemon;
		}
	}
}


t_log* iniciar_logger(char* path)
{
	t_log* logger;
	if((logger = log_create(path,"team",true,LOG_LEVEL_INFO)) == NULL)
	{
		printf("No se pudo crear el log\n");
		exit(1);
	}
	return logger;
}

t_config* leer_config(void)
{
	t_config *config;//Puede que este mal el path
	if((config = config_create("src/team.config")) == NULL)
	{
		printf("No pude leer la config\n");
		exit(2);
	}
	return config;
}

void terminar_programa(t_log* logger, t_config* config){

	if(logger != NULL)
	{
		log_info(logger, "finalizando programa...");
		log_destroy(logger);
	}
	if(config != NULL)
	{
		config_destroy(config);
	}
}

