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

	//Leer archivo config y settear variables
	config = leer_config();

	entrenadores = leer_entrenadores(config);
	objetivo_global = obtener_objetivo_global(entrenadores);

	tiempo_de_reconexion = config_get_int_value(config, "TIEMPO_RECONEXION");
	retardo_cpu = config_get_int_value(config, "RETARDO_CICLO_CPU");

	char* algoritmo_planificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	uint32_t quantum = config_get_int_value(config, "QUANTUM");
	uint32_t estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");
	planificador = planificador_create(algoritmo_planificacion, quantum, estimacion_inicial);

	ip_broker = config_get_string_value(config, "IP_BROKER");
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");

	char* path_logger = config_get_string_value(config, "LOG_FILE");
	logger = iniciar_logger(path_logger);
/*



	//Conectarse a las colas del broker
	iniciar_conexion_broker(ip_broker, puerto_broker);

	//Enviar mensaje GET al broker segun objetivos globales
	enviar_get_objetivo(objetivo_global);

	//Abrir socket de escucha para el Gameboy
	iniciar_puerto_de_escucha();
*/
	test();


	terminar_programa(logger, config);
}

void test(){
	//Test de verdad
	//Inicio los hilos de los entrenadores
	for(int i = 0 ; i < list_size(entrenadores) ; i++){
		planificador_iniciar_hilo_entrenador(list_get(entrenadores, i));
	}

	//nuevo APPEARED
	t_appeared_pokemon* msj = appeared_pokemon_create("Pikachu", 5, 3);
	//agarramos el pokemon
	t_pokemon* pokemon = pokemon_create(msj->nombre, msj->posicion);
	//entrenador_entrar_a_planificacion(pokemon)
	entrenador_entrar_a_planificacion(pokemon);

	//nuevo APPEARED
	t_appeared_pokemon* nuevo_msj = appeared_pokemon_create("Charmander", 1, 3);
	//agarramos el pokemon
	t_pokemon* nuevo_pokemon = pokemon_create(nuevo_msj->nombre, nuevo_msj->posicion);
	//entrenador_entrar_a_planificacion(pokemon)
	entrenador_entrar_a_planificacion(nuevo_pokemon);

	//Deberia pasar:

	//Encolado entrenador en [5,5]
	//Entrenador en [5,5] pasa a ejecucion
	//Encolado entrenador en [1,2]
	// .. 2 seg ..
	//Me movi a [5, 4]
	// .. 2 seg ..
	//Me movi a [5, 3]
	//Llegue a destino, entrenador bloqueado
	//Entrenador en [1,2] pasa a ejecucion
	// .. 2 seg ..
	//Me movi a [1, 3]
	//Llegue a destino, entrenador bloqueado


	for(int i = 0 ; i < list_size(entrenadores) ; i++){
		t_entrenador* ent = list_get(entrenadores, i);
		pthread_join(ent->hilo, NULL);
	}

	printf("\n\n");
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

