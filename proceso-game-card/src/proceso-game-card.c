/*
 ============================================================================
 Name        : proceso-game-card.c
 Author      : Grupo 7 - SO
 Version     : v1.0
 Copyright   : Your copyright notice
 Description : Almacena los datos los datos de los distintos Pokemon que se
               encuentren en el mapa.
 ============================================================================
 */

#include "proceso-game-card.h"

int main(void) {
	inicializar_variables_globales();
	inicializar_filesystem();
	//iniciar_conexion_broker();
	//iniciar_puerto_escucha();

	terminar_programa();
}

void inicializar_variables_globales(){
	config = leer_config();

	tiempo_reintento_conexion = config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION");
	tiempo_reintento_operacion = config_get_int_value(config, "TIEMPO_DE_REINTENTO_OPERACION");
	tiempo_retardo_operacion = config_get_int_value(config, "TIEMPO_RETARDO_OPERACION");
	punto_de_montaje_tallgrass = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
	ip_broker = config_get_string_value(config, "IP_BROKER");
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");
	iniciar_logger(config_get_string_value(config, "LOG_FILE"));
}

void inicializar_filesystem(){

}

t_log* iniciar_logger(char* path){
	t_log* logger;
	if((logger = log_create(path,"gamecard",true,LOG_LEVEL_INFO)) == NULL)
	{
		printf("No se pudo crear el log\n");
		exit(1);
	}
	return logger;
}

t_config* leer_config(void){
	t_config *config;//Puede que este mal el path
	if((config = config_create("src/gamecard.config")) == NULL)//Nota: para correr desde Debug
	{														//hay que agregar ../ al path
		printf("No pude leer la config\n");
		exit(2);
	}
	return config;
}

void terminar_programa(){

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

