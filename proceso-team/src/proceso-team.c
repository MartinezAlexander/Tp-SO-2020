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
	iniciar_conexion_broker();

	//Enviar mensaje GET al broker segun objetivos globales
	enviar_get_objetivo(objetivo_global);

	//Abrir socket de escucha para el Gameboy
	iniciar_puerto_de_escucha();
*/

//	test();


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
	//entrenador_entrar_a_planificacion(pokemon)
	entrenador_entrar_a_planificacion(msj->pokemon);

	//nuevo APPEARED
	t_appeared_pokemon* nuevo_msj = appeared_pokemon_create("Charmander", 1, 3);
	//entrenador_entrar_a_planificacion(pokemon)
	entrenador_entrar_a_planificacion(nuevo_msj->pokemon);

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

void iniciar_conexion_broker(){
	socket_appeared = iniciar_suscripcion_broker(APPEARED_POKEMON);
	socket_caught = iniciar_suscripcion_broker(CAUGHT_POKEMON);
	socket_localized = iniciar_suscripcion_broker(LOCALIZED_POKEMON);

	pthread_t hilo_cola_appeared;
	pthread_t hilo_cola_caught;
	pthread_t hilo_cola_localized;

	pthread_create(&hilo_cola_appeared, NULL, recibir_mensaje_appeared, socket_appeared);
	pthread_create(&hilo_cola_caught, NULL, recibir_mensaje_caught, socket_caught);
	pthread_create(&hilo_cola_localized, NULL, recibir_mensaje_localized, socket_localized);
}

void recibir_mensaje_appeared(int socket_appeared){
	while(1){
		t_mensaje* mensaje_appeared = recibir_mensaje(socket_appeared);
		//TODO mandar ack al broker
		procesar_appeared((t_appeared_pokemon*) mensaje_appeared->mensaje);
	}
}

void recibir_mensaje_caught(int socket_caught){
	while(1){
		t_mensaje* mensaje_caught = recibir_mensaje(socket_caught);
		//TODO mandar ack al broker
		procesar_caught((t_caught_pokemon*) mensaje_caught->mensaje, mensaje_caught->id_correlativo);
	}
}

void recibir_mensaje_localized(int socket_localized){
	while(1){
		t_mensaje* mensaje_localized = recibir_mensaje(socket_localized);
		//TODO mandar ack al broker

		procesar_localized((t_localized_pokemon*) mensaje_localized->mensaje);
	}
}

int iniciar_suscripcion_broker(op_code cola){
	t_suscripcion* suscripcion = suscripcion_proceso_create(TEAM, getpid(), cola);
	t_mensaje* mensaje = mensaje_simple_create((void*) suscripcion, SUSCRIPCION);

	int socket = crear_conexion(ip_broker, puerto_broker);
	enviar_mensaje(mensaje, socket);

	return socket;
}

void iniciar_puerto_de_escucha(){
	//TODO definir ip y puerto del server por config por ahi??
	char* ip = "127.0.0.2";
	char* puerto = "5002";
	iniciar_servidor(ip, puerto,(void*) procesar_mensajes_directos);
}

//Se encarga de procesar todos los mensajes que lleguen directamente de un gameboy
void procesar_mensajes_directos(int* socket){
	t_mensaje* mensaje = recibir_mensaje(socket);

	switch(mensaje->codigo){
		case APPEARED_POKEMON:
			procesar_appeared((t_appeared_pokemon*)mensaje->mensaje);
			break;
		case CAUGHT_POKEMON:
			procesar_caught((t_caught_pokemon*)mensaje->mensaje, mensaje->id_correlativo);
			break;
		case LOCALIZED_POKEMON:
			procesar_localized((t_localized_pokemon*)mensaje->mensaje);
			break;
		default:
			printf("\n--Mensaje directo recibido no procesable--\n");
			break;
	}
}

void procesar_localized(t_localized_pokemon* localized_pokemon){

}

void procesar_appeared(t_appeared_pokemon* appeared_pokemon){
	printf("Procesando mensaje APPEARED\n");
	printf("Recibo %s\n\n", appeared_pokemon->pokemon->especie);

	if(cumplio_objetivo_global(objetivo_global, entrenadores)){
		//free(appeared_pokemon);
	}else if(pokemon_dentro_de_objetivos(objetivo_global, appeared_pokemon->pokemon->especie)){
		//TODO Verifico si ya estoy procesando un localized de la misma especie
		//Planifico entrenador
		entrenador_entrar_a_planificacion(appeared_pokemon->pokemon);
	}else{
		//free(appeared_pokemon);
	}
}

void procesar_caught(t_caught_pokemon* caught_pokemon, int32_t id_correlativo){

	char* key_id = string_itoa(id_correlativo);
	//Validar id mensaje
	if(!dictionary_has_key(mensajes_catch_pendientes, key_id)){
		//TODO Descarto en caso de que no sea valido
	}else{
		t_entrenador* entrenador = dictionary_get(mensajes_catch_pendientes, key_id);
		//Valido resultado del caught
		if(caught_pokemon->atrapado){

			entrenador_atrapar_objetivo(entrenador);

			//Actualizo el estado del entrenador
			if(cumplio_objetivo_entrenador(entrenador)){
				entrenador->estado = EXIT;
			}else{
				if(entrenador_estado_deadlock(entrenador)){
					entrenador->estado = BLOCKED_DEADLOCK;
				}else{
					//Caso si puedo seguir atrapando
					encolar(entrenador);
				}
			}
		}else{
			//Caso contratio queda bloqueado
			//(habria que ver que tipo de bloqueo, ya que ahora no esta esperando una rta)
			entrenador_resetear_objetivo(entrenador);
			entrenador->estado = BLOCKED;
		}

		dictionary_remove(mensajes_catch_pendientes, key_id);
	}
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

