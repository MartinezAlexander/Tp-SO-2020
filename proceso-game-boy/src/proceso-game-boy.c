/*
 ============================================================================
 Name        : proceso-game-boy.c
 Author      : Grupo 7 - SO
 Version     : v1.0
 Copyright   : Your copyright notice
 Description : Proceso game boy sirve para testear los demas procesos por
 separado.
 ============================================================================
 */

#include "proceso-game-boy.h"

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

t_mensaje* procesar_mensaje(char** mensaje, op_code codigo, t_proceso id) {
	void* mensaje_creado = NULL;
	t_mensaje* mensaje_procesado = NULL;

	if (id == BROKER && codigo == NEW_POKEMON) {
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(mensaje[5]);
		uint32_t cantidad = (uint32_t) atoi(mensaje[6]);
		mensaje_creado = (void*) new_pokemon_create(mensaje[3], x, y, cantidad);
		mensaje_procesado = mensaje_simple_create(mensaje_creado, codigo);
	}

	if (id == BROKER && codigo == APPEARED_POKEMON) {
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(mensaje[5]);
		int32_t id_c = atoi(mensaje[6]);
		mensaje_creado = (void*) appeared_pokemon_create(mensaje[3], x, y);
		mensaje_procesado = mensaje_con_id_correlativo_create(mensaje_creado,
				codigo, id_c);
	}

	if (id == BROKER && codigo == CATCH_POKEMON) {
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(mensaje[5]);
		mensaje_creado = (void*) catch_pokemon_create(mensaje[3], x, y);
		mensaje_procesado = mensaje_simple_create(mensaje_creado, codigo);
	}

	if (id == BROKER && codigo == CAUGHT_POKEMON) {
		int32_t id_c = atoi(mensaje[3]);
		uint32_t situacion = atrapo_pokemon(mensaje[4]);
		mensaje_creado = (void*) caught_pokemon_create(situacion);
		mensaje_procesado = mensaje_con_id_correlativo_create(mensaje_creado,
				codigo, id_c);
	}

	if (id == BROKER && codigo == GET_POKEMON) {
		mensaje_creado = (void*) get_pokemon_create(mensaje[3]);
		mensaje_procesado = mensaje_simple_create(mensaje_creado, codigo);
	}

	if (id == TEAM && codigo == APPEARED_POKEMON) {
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(mensaje[5]);
		mensaje_creado = (void*) appeared_pokemon_create(mensaje[3], x, y);
		mensaje_procesado = mensaje_simple_create(mensaje_creado, codigo);
	}

	if (id == GAMECARD && codigo == NEW_POKEMON) {
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(mensaje[5]);
		uint32_t cantidad = (uint32_t) atoi(mensaje[6]);
		int32_t id_m = atoi(mensaje[7]);
		mensaje_creado = (void*) new_pokemon_create(mensaje[3], x, y, cantidad);
		mensaje_procesado = mensaje_con_id_create(mensaje_creado, codigo, id_m);
	}

	if (id == GAMECARD && codigo == CATCH_POKEMON) {
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(mensaje[5]);
		int32_t id_m = atoi(mensaje[6]);
		mensaje_creado = (void*) catch_pokemon_create(mensaje[3], x, y);
		mensaje_procesado = mensaje_con_id_create(mensaje_creado, codigo, id_m);
	}

	if (id == GAMECARD && codigo == GET_POKEMON) {
		int32_t id_m = atoi(mensaje[4]);
		mensaje_creado = (void*) get_pokemon_create(mensaje[3]);
		mensaje_procesado = mensaje_con_id_create(mensaje_creado, codigo, id_m);
	}

	return mensaje_procesado;
}

void enviar_a(t_proceso id, t_mensaje* mensaje) {
	int socket;
	switch (id) {
	case BROKER:
		socket = crear_conexion(ip_broker, puerto_broker);
		enviar_mensaje(mensaje, socket);
		liberar_conexion(socket);
		break;
	case TEAM:
		socket = crear_conexion(ip_team, puerto_team);
		enviar_mensaje(mensaje, socket);
		liberar_conexion(socket);
		break;
	case GAMECARD:
		socket = crear_conexion(ip_gamecard, puerto_gamecard);
		enviar_mensaje(mensaje, socket);
		liberar_conexion(socket);
		break;
	case SUSCRIPTOR:
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

	char* path_logger = config_get_string_value(config, "LOG_FILE");

	logger = iniciar_logger(path_logger);
}

int main(int arg, char** args) {

	config = leer_config();

	inicializar_variables();

	t_proceso id_proceso = obtener_id_proceso(args[1]);
	op_code tipo_mensaje = obtener_tipo_mensaje(args[2]);
	t_mensaje* mensaje_procesado = procesar_mensaje(args, tipo_mensaje,
			id_proceso);
	enviar_a(id_proceso, mensaje_procesado);
	return 0;
}

t_log* iniciar_logger(char* path) {

	t_log* logger;

	if ((logger = log_create(path, "gameboy", true, LOG_LEVEL_INFO)) == NULL) {
		printf("No se pudo crear el log\n");
		exit(1);
	}

	return logger;
}

t_config* leer_config(void) {

	t_config* config =
			config_create(
					"/home/utnso/workspace/tp-2020-1c-Grupo-7-SO/proceso-game-boy/gameboy.config");

	if (config == NULL) {
		printf("No puede leer la config\n");
		exit(2);
	}
	return config;
}

void terminar_programa(t_log* logger, t_config* config) {

	if (logger != NULL) {
		log_info(logger, "finalizando programa...");
		log_destroy(logger);
	}

	if (config != NULL) {
		config_destroy(config);
	}
}

