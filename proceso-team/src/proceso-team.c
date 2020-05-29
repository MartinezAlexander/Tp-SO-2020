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


int main(void) {
	inicializar_variables();

	iniciar_hilos_entrenadores();

	//Me suscribo a las colas y abro hilos para recibir mensajes
	//iniciar_conexion_broker();
	//Envio mensaje GET al broker segun objetivos globales
	//enviar_get_objetivo(objetivo_global);
	//Abro socket de escucha para el Gameboy
	//iniciar_puerto_de_escucha();

	test();

	//Antes de terminar el programa, debo esperar a que
	//terminen de ejecutar todos los entrenadores (hilos)
	//Cuando esto ocurra, tambien significara que el
	//proceso team termino.
	esperar_hilos_entrenadores();

	//TODO hay que ver como apagar los hilos de escucha
	//del broker, una vez que termino el team
	//
	terminar_programa(logger, config);

}

//TODO (estadisticas) ciclos de CPU totales (se calcula al final)
//TODO (estadisticas) Cantidad de cambios de contexto realizados
//TODO (estadisticas) Deadlocks producidos y resueltos

void inicializar_variables(){
	config = leer_config();

	char* algoritmo_planificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	uint32_t quantum = config_get_int_value(config, "QUANTUM");
	uint32_t estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");
	double alpha = config_get_double_value(config, "ALPHA");

	planificador = planificador_create(algoritmo_planificacion, quantum, estimacion_inicial, alpha);

	entrenadores = leer_entrenadores(config, estimacion_inicial);
	objetivo_global = obtener_objetivo_global(entrenadores);
	diccionario_especies_recibidas = inicializar_diccionario_especies();

	tiempo_de_reconexion = config_get_int_value(config, "TIEMPO_RECONEXION");
	retardo_cpu = config_get_int_value(config, "RETARDO_CICLO_CPU");

	ip_broker = config_get_string_value(config, "IP_BROKER");
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");

	char* path_logger = config_get_string_value(config, "LOG_FILE");
	logger = iniciar_logger(path_logger);

	ip_team = config_get_string_value(config, "IP_TEAM");
	puerto_team = config_get_string_value(config, "PUERTO_TEAM");
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
