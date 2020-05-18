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

#include <stdio.h>
#include <stdlib.h>
#include <mensajes/client.h>
#include <mensajes/mensajes.h>
#include <mensajes/appeared_pokemon.h>
#include <mensajes/new_pokemon.h>
#include <mensajes/get_pokemon.h>
#include <mensajes/catch_pokemon.h>
#include <mensajes/caught_pokemon.h>
#include <mensajes/localized_pokemon.h>
#include <commons/string.h>

typedef enum{
	BROKER = 1,
	TEAM,
	GAMECARD,
	SUSCRIPTOR
}t_proceso;

t_proceso obtener_id_proceso(char* id){
	t_proceso id_proceso;
	if(string_equals_ignore_case(id,"BROKER")){
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

op_code obtener_tipo_mensaje(char* tipo){
	op_code codigo;
	if (string_equals_ignore_case(tipo, "NEW_POKEMON")) {
		codigo = NEW_POKEMON;
	}
	if (string_equals_ignore_case(tipo, "APPEARED_POKEMON")) {
		codigo  = APPEARED_POKEMON;
	}
	if (string_equals_ignore_case(tipo, "GET_POKEMON")) {
		codigo  = GET_POKEMON;
	}
	if (string_equals_ignore_case(tipo, "CATCH_POKEMON")) {
		codigo  = CATCH_POKEMON;
	}
	if (string_equals_ignore_case(tipo, "CAUGHT_POKEMON")) {
		codigo = CAUGHT_POKEMON;
	}
	if (string_equals_ignore_case(tipo, "LOCALIZED_POKEMON")) {
		codigo = LOCALIZED_POKEMON;
	}
	return codigo ;
}

void* procesar_mensaje(char** mensaje, op_code codigo, t_proceso id){
	void* mensaje_procesado = NULL;

	if(codigo == NEW_POKEMON && id==BROKER){
		uint32_t x = (uint32_t)atoi(mensaje[4]);
		uint32_t y = (uint32_t)atoi(mensaje[5]);
		uint32_t cantidad = (uint32_t)atoi(mensaje[6]);
		mensaje_procesado = (void*)new_pokemon_create(mensaje[3],x,y,cantidad);
	}

	if(codigo == APPEARED_POKEMON && id==BROKER){
		//crear mensaje appeared y agregarle un id
	}

	if(codigo == CATCH_POKEMON && id==BROKER){
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(mensaje[5]);
		mensaje_procesado = (void*)catch_pokemon_create(mensaje[3],x,y);
	}

	if (codigo == CAUGHT_POKEMON && id == BROKER) {
		// crear mensaje caught y agregar id
	}

	if (codigo == GET_POKEMON && id == BROKER) {
		mensaje_procesado = (void*)get_pokemon_create(mensaje[3]);
	}

	if (codigo == APPEARED_POKEMON && id == TEAM) {
		uint32_t x = (uint32_t) atoi(mensaje[4]);
		uint32_t y = (uint32_t) atoi(mensaje[5]);
		mensaje_procesado = (void*)appeared_pokemon_create(mensaje[3], x, y);
	}

	if (codigo == NEW_POKEMON && id == GAMECARD) {
		//crear un new pokemon y agregarle el id
	}

	if (codigo == CATCH_POKEMON && id == GAMECARD) {
		//crear un catch pokemon y agregarle un id
	}

	if (codigo == GET_POKEMON && id == GAMECARD) {
		mensaje_procesado = (void*)get_pokemon_create(mensaje[3]);
	}

	return mensaje_procesado;
}

//Debemos leerlo del archivo de configuracion
#define IP_BROKER "127.0.0.1"
#define IP_TEAM "127.0.0.2"
#define IP_GAMECARD "127.0.0.3"
#define PUERTO_BROKER "5003"
#define PUERTO_TEAM "5002"
#define PUERTO_GAMECARD "5001"

void enviar_a(t_proceso id, void* mensaje, op_code codigo){
	int socket;
	switch(id){
	case BROKER:
		socket = crear_conexion(IP_BROKER,PUERTO_BROKER);
		enviar_mensaje(mensaje,codigo,socket);
		liberar_conexion(socket);
		break;
	case TEAM:
		socket = crear_conexion(IP_TEAM, PUERTO_TEAM);
		enviar_mensaje(mensaje, codigo, socket);
		liberar_conexion(socket);
		break;
	case GAMECARD:
		socket = crear_conexion(IP_GAMECARD, PUERTO_GAMECARD);
		enviar_mensaje(mensaje, codigo, socket);
		liberar_conexion(socket);
		break;
	case SUSCRIPTOR:
		break;
	default:
		break;
	}
}

int main(int arg, char** args){
	t_proceso id_proceso = obtener_id_proceso(args[1]);
	op_code tipo_mensaje = obtener_tipo_mensaje(args[2]);
	void* mensaje_procesado = procesar_mensaje(args,tipo_mensaje, id_proceso);
	enviar_a(id_proceso,mensaje_procesado, tipo_mensaje);
	return 0;
}
