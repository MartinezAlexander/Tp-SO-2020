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

	iniciar_hilos_planificacion();

	//Me suscribo a las colas y abro hilos para recibir mensajes
	iniciar_conexion_broker();
	//Envio mensaje GET al broker segun objetivos globales
	enviar_get_objetivo(objetivo_global);
	//Abro socket de escucha para el Gameboy
	iniciar_puerto_de_escucha();

	//test_sjf_con_desalojo();

	//test_deadlock2();

	//Antes de terminar el programa, debo esperar a que
	//terminen de ejecutar todos los entrenadores (hilos)
	//Cuando esto ocurra, tambien significara que el
	//proceso team termino.
	esperar_hilos_planificacion();
	//Una vez que llego a esta zona se que ya todos los entrenadores
	//estan en estado exit, por lo que puedo liberar las conexiones
	//con el broker y joinear los hilos que llevaban a cabo
	//la escucha y el procesamiento de mensajes
	cerrar_conexion_broker();

	int ciclos_cpu_totales = 0;
	for(int i = 0 ; i < list_size(entrenadores) ; i++){
		t_entrenador* entrenador = list_get(entrenadores, i);
		int ciclos = (int)dictionary_get(diccionario_ciclos_entrenador, string_itoa(entrenador->identificador));
		ciclos_cpu_totales += ciclos;
	}

	//Loggear estadisticas

	//quedaria liberar variables globales
	terminar_programa(logger, config);
}

//TODO (estadisticas) Deadlocks producidos y resueltos

void inicializar_variables(){
	config = leer_config();

	finalizo_team = 0;
	cambios_de_contexto = 0;

	char* algoritmo_planificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	uint32_t quantum = config_get_int_value(config, "QUANTUM");

	uint32_t estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");

	double alpha = config_get_double_value(config, "ALPHA");

	planificador = planificador_create(algoritmo_planificacion, quantum, estimacion_inicial, alpha);

	entrenadores = leer_entrenadores(config, estimacion_inicial);

	objetivo_global = obtener_objetivo_global(entrenadores);

	diccionario_especies_recibidas = inicializar_diccionario_especies();
	pthread_mutex_init(&mutex_diccionario_especies, NULL);

	cola_pokemones_en_espera = queue_create();
	pthread_mutex_init(&mutex_cola_espera, NULL);

	pthread_mutex_init(&mutex_procesamiento_pokemon, NULL);

	tiempo_de_reconexion = config_get_int_value(config, "TIEMPO_RECONEXION");
	retardo_cpu = config_get_int_value(config, "RETARDO_CICLO_CPU");

	ip_broker = config_get_string_value(config, "IP_BROKER");
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");

	team_id = config_get_int_value(config, "TEAM_ID");

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
	t_config *config;
	if((config = config_create("../src/team.config")) == NULL)//Nota: para correr desde Debug
	{														//hay que agregar ../ al path
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
