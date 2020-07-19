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


int main(int arg, char** args) {
	inicializar_variables((char*)args[1]);

	iniciar_hilos_planificacion();

	//Me suscribo a las colas y abro hilos para recibir mensajes
	iniciar_conexion_broker();
	//Envio mensaje GET al broker segun objetivos globales
	enviar_get_objetivo(objetivo_global);
	//Abro socket de escucha para el Gameboy
	iniciar_puerto_de_escucha();

	/*
	 * Tengo que esperar a que me llegue señal de que mis entrenadores estan
	 * en EXIT o en DEADLOCK, para poder resolverlo en caso de que sea necesario
	 */
	sem_wait(&semaforo_resolucion_deadlock);

	puts("\n ------------------------------------------ \n");
	resolver_deadlock_nuevo();

	//Antes de terminar el programa, debo esperar a que
	//terminen de ejecutar todos los entrenadores (hilos)
	//Cuando esto ocurra, tambien significara que el
	//proceso team termino.
	esperar_hilos_planificacion();
	printf("[Team] Todos sus entrenadores han cumplido su objetivo\n");

	puts("\n ------------------------------------------ \n");
	loggear_resultado_team();

	//quedaria liberar variables globales
	terminar_programa(logger, config);
}

void inicializar_variables(char* path_config){
	config = leer_config(path_config);

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

	sem_init(&semaforo_resolucion_deadlock, 0, 0);

	tiempo_de_reconexion = config_get_int_value(config, "TIEMPO_RECONEXION");
	retardo_cpu = config_get_int_value(config, "RETARDO_CICLO_CPU");

	ip_broker = config_get_string_value(config, "IP_BROKER");
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");

	team_id = config_get_int_value(config, "TEAM_ID");

	char* path_logger = config_get_string_value(config, "LOG_FILE");
	logger = iniciar_logger(path_logger);

	ip_team = config_get_string_value(config, "IP_TEAM");
	puerto_team = config_get_string_value(config, "PUERTO_TEAM");

	//config_destroy(config);
}

t_log* iniciar_logger(char* path)
{
	t_log* logger;
	if((logger = log_create(path,"team",false,LOG_LEVEL_INFO)) == NULL)
	{
		printf("[Logger] No se pudo crear el log\n");
		exit(1);
	}
	return logger;
}

t_config* leer_config(char* path_config)
{
	t_config *config;
	if((config = config_create(path_config)) == NULL)//Nota: para correr desde Debug hay que agregar ../ al path
	{
		printf("[Config] No pude leer la config\n");
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

	//liberar variables globales
	sem_destroy(&semaforo_planificacion);
	list_destroy(objetivo_global);
	free(ip_broker);
	free(ip_team);
	free(puerto_broker);
	free(puerto_team);

	pthread_mutex_destroy(&mutex_diccionario_especies);
	pthread_mutex_destroy(&mutex_cola_espera);
	pthread_mutex_destroy(&mutex_cola_espera);

	dictionary_destroy(diccionario_especies_recibidas);
	dictionary_destroy(diccionario_ciclos_entrenador);
	queue_destroy(cola_pokemones_en_espera);

	planificador_destroy(planificador);

	//entrenadores_destroy();

	conexion_broker_destroy();

	printf("Finaliza el programa\n");
}
