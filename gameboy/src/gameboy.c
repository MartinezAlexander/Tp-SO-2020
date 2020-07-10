/*
 ============================================================================
 Name        : gameboy.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "gameboy.h"

t_proceso obtener_id_proceso(char* id) {
	t_proceso id_proceso;
	if (string_equals_ignore_case(id, "BROKER")) {
		id_proceso = BROKER;
	}
	if (string_equals_ignore_case(id, "TEAM")) {
		id_proceso = TEAM;
	}
	if (string_equals_ignore_case(id, "GAMECARD")) {
		id_proceso = GAMECARD;
	}
	if (string_equals_ignore_case(id, "SUSCRIPTOR")) {
		id_proceso = SUSCRIPTOR;
	}
	return id_proceso;
}

op_code obtener_tipo_mensaje(char* tipo) {
	op_code codigo;
	if (string_equals_ignore_case(tipo, "NEW_POKEMON")) {
		codigo = NEW_POKEMON;
	}
	if (string_equals_ignore_case(tipo, "APPEARED_POKEMON")) {
		codigo = APPEARED_POKEMON;
	}
	if (string_equals_ignore_case(tipo, "GET_POKEMON")) {
		codigo = GET_POKEMON;
	}
	if (string_equals_ignore_case(tipo, "CATCH_POKEMON")) {
		codigo = CATCH_POKEMON;
	}
	if (string_equals_ignore_case(tipo, "CAUGHT_POKEMON")) {
		codigo = CAUGHT_POKEMON;
	}
	if (string_equals_ignore_case(tipo, "LOCALIZED_POKEMON")) {
		codigo = LOCALIZED_POKEMON;
	}
	return codigo;
}

uint32_t atrapo_pokemon(char* confirmacion) {
	uint32_t atrapado;
	if (string_equals_ignore_case(confirmacion, "OK")) {
		atrapado = 1;
	}
	if (string_equals_ignore_case(confirmacion, "FAIL")) {
		atrapado = 0;
	}
	return atrapado;
}

void desconectar_suscriptor() {
	sleep(tiempo_conexion);
	exit(0);
}

char* nombre_sin_barra_n(char* mensaje){
	if(string_ends_with(mensaje,"\n")){
		char* pokemon = string_substring(mensaje,0,(string_length(mensaje)-2));
		printf("Nombre pokemon [%s]",pokemon);
		return pokemon;
	}
	else{
		printf("Nombre pokemon [%s]",mensaje);
		return mensaje;
	}
}

int es_caracter_valido(char c){
	int valido = 0;
	if(c > 64 && c < 91){
		valido = 1;
	}
	if(c > 47 && c < 58){
		valido = 1;
	}
	if(c == 95){
		valido = 1;
	}
	if(c > 96 && c < 123){
		valido = 1;
	}
	return valido;
}

char* filtrar_string_dejar_solo_caracteres_validos(char* mensaje){
	char* string = string_new();
	for(int i = 0; i < string_length(mensaje); i++){
		if(es_caracter_valido(mensaje[i])){
			string = string_from_format("%s%c",string,mensaje[i]);
		}
	}
	return string;
}

t_mensaje* procesar_mensaje(char** mensaje, op_code codigo, t_proceso id) {
	void* mensaje_creado = NULL;
	t_mensaje* mensaje_procesado = NULL;

	if (id == BROKER && codigo == NEW_POKEMON) {
		char* cantidad_string = filtrar_string_dejar_solo_caracteres_validos(mensaje[6]);
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(mensaje[5]);
		uint32_t cantidad = (uint32_t) atoi(cantidad_string);
		mensaje_creado = (void*) new_pokemon_create(mensaje[3], x, y, cantidad);
		mensaje_procesado = mensaje_simple_create(mensaje_creado, codigo);
	}

	if (id == BROKER && codigo == APPEARED_POKEMON) {
		char* id_c_string = filtrar_string_dejar_solo_caracteres_validos(mensaje[6]);
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(id_c_string);
		int32_t id_c = atoi(mensaje[6]);
		mensaje_creado = (void*) appeared_pokemon_create(mensaje[3], x, y);
		mensaje_procesado = mensaje_con_id_correlativo_create(mensaje_creado,codigo, id_c);
	}

	if (id == BROKER && codigo == CATCH_POKEMON) {
		char* y_string = filtrar_string_dejar_solo_caracteres_validos(mensaje[5]);
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(y_string);
		mensaje_creado = (void*) catch_pokemon_create(mensaje[3], x, y);
		mensaje_procesado = mensaje_simple_create(mensaje_creado, codigo);
	}

	if (id == BROKER && codigo == CAUGHT_POKEMON) {
		char* situacion_string = filtrar_string_dejar_solo_caracteres_validos(mensaje[4]);
		int32_t id_c = atoi(mensaje[3]);
		uint32_t situacion = atrapo_pokemon(situacion_string);
		mensaje_creado = (void*) caught_pokemon_create(situacion);
		mensaje_procesado = mensaje_con_id_correlativo_create(mensaje_creado,codigo, id_c);
	}

	if (id == BROKER && codigo == GET_POKEMON) {
		char* nombre = filtrar_string_dejar_solo_caracteres_validos(mensaje[3]);
		mensaje_creado = (void*) get_pokemon_create(nombre);
		mensaje_procesado = mensaje_simple_create(mensaje_creado, codigo);
	}

	if (id == TEAM && codigo == APPEARED_POKEMON) {
		char* string = filtrar_string_dejar_solo_caracteres_validos(mensaje[5]);
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(string);
		mensaje_creado = (void*) appeared_pokemon_create(mensaje[3], x, y);
		mensaje_procesado = mensaje_simple_create(mensaje_creado, codigo);
	}

	if (id == GAMECARD && codigo == NEW_POKEMON) {
		char* id_m_string = filtrar_string_dejar_solo_caracteres_validos(mensaje[7]);
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(mensaje[5]);
		uint32_t cantidad = (uint32_t) atoi(mensaje[6]);
		int32_t id_m = atoi(id_m_string);
		mensaje_creado = (void*) new_pokemon_create(mensaje[3], x, y, cantidad);
		mensaje_procesado = mensaje_con_id_create(mensaje_creado, codigo, id_m);
	}

	if (id == GAMECARD && codigo == CATCH_POKEMON) {
		char* id_m_string = filtrar_string_dejar_solo_caracteres_validos(mensaje[6]);
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(mensaje[5]);
		int32_t id_m = atoi(id_m_string);
		mensaje_creado = (void*) catch_pokemon_create(mensaje[3], x, y);
		mensaje_procesado = mensaje_con_id_create(mensaje_creado, codigo, id_m);
	}

	if (id == GAMECARD && codigo == GET_POKEMON) {
		char* id_m_string = filtrar_string_dejar_solo_caracteres_validos(mensaje[4]);
		int32_t id_m = atoi(id_m_string);
		mensaje_creado = (void*) get_pokemon_create(mensaje[3]);
		mensaje_procesado = mensaje_con_id_create(mensaje_creado, codigo, id_m);
	}
	if (id == SUSCRIPTOR) {
		t_suscripcion* mensaje_suscripcion = suscripcion_proceso_create(id,gameboy_id, codigo);
		mensaje_procesado = mensaje_simple_create((void*) mensaje_suscripcion,SUSCRIPCION);
	}
	return mensaje_procesado;
}

void enviar_a(t_proceso id, t_mensaje* mensaje) {
	int socket;
	switch (id) {
	case BROKER:
		socket = crear_conexion(ip_broker, puerto_broker);
		loggear_conexion(id,socket);
		enviar_mensaje(mensaje, socket);
		recibir_id(socket);
		liberar_conexion(socket);
		break;
	case TEAM:
		socket = crear_conexion(ip_team, puerto_team);
		enviar_mensaje(mensaje, socket);
		loggear_conexion(id,socket);
		recibir_ACK(socket);
		liberar_conexion(socket);
		break;
	case GAMECARD:
		socket = crear_conexion(ip_gamecard, puerto_gamecard);
		enviar_mensaje(mensaje, socket);
		loggear_conexion(id,socket);
		recibir_ACK(socket);
		liberar_conexion(socket);
		break;
	default:
		break;
	}
}
void inicializar_variables() {
	ip_broker = config_get_string_value(config, "IP_BROKER");
	ip_team = config_get_string_value(config, "IP_TEAM");
	ip_gamecard = config_get_string_value(config, "IP_GAMECARD");
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");
	puerto_team = config_get_string_value(config, "PUERTO_TEAM");
	puerto_gamecard = config_get_string_value(config, "PUERTO_GAMECARD");

	path_logger = config_get_string_value(config, "LOG_FILE");

	gameboy_id = config_get_int_value(config,"GAMEBOY_ID");
}
int main(int arg, char** args) {

	config = leer_config();
	inicializar_variables();
	iniciar_logger(path_logger);

	t_proceso id_proceso = obtener_id_proceso(args[1]);

	op_code tipo_mensaje = obtener_tipo_mensaje(args[2]);
	t_mensaje* mensaje_procesado = procesar_mensaje(args, tipo_mensaje,id_proceso);

	if (id_proceso == SUSCRIPTOR) {
		char* tiempo_string = filtrar_string_dejar_solo_caracteres_validos(args[3]);
		tiempo_conexion = atoi(tiempo_string);
		int socket = crear_conexion(ip_broker, puerto_broker);
		loggear_conexion(id_proceso,socket);
		enviar_mensaje(mensaje_procesado, socket);
		int estoy_suscripto = recibir_confirmacion_suscripcion(socket);

		if (estoy_suscripto) {

			loggear_suscripcion(tipo_mensaje);
			pthread_t suscriptor_desconexion;

			pthread_create(&suscriptor_desconexion, NULL,(void*) desconectar_suscriptor, NULL);

			while (1) {
				t_mensaje* mensaje = recibir_mensaje(socket);
				if (mensaje==NULL){
					loggear_desconexion(id_proceso);
					tiempo_conexion = 0 ;
					desconectar_suscriptor();
				}
				else{
				enviar_ACK(socket);
				loggear_nuevo_mensaje(tipo_mensaje,mensaje_to_string(mensaje));
				mensaje_mostrar(mensaje);
				}
			}
		}

	} else {
		enviar_a(id_proceso, mensaje_procesado);
	}
	return 0;
}


t_config* leer_config(void) {

	t_config* config = config_create("../src/gameboy.config");

	if (config == NULL) {
		exit(2);
	}
	return config;
}


void terminar_programa(t_log* logger, t_config* config) {

	if (logger != NULL) {
		log_destroy(logger);
	}

	if (config != NULL) {
		config_destroy(config);
	}
}
