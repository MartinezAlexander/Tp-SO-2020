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

int main(void) {
	//Prueba de conexion entre proceso team y proceso game boy

	//Creo un socket para conectarme al proceso team
	int socket1 = crear_conexion("127.0.0.2","5002");

	//Envio el mensaje t_appeared_pokemon al proceso team
	t_appeared_pokemon* appeared_mensaje1 = appeared_pokemon_create("pikachu",2,3);
	enviar_mensaje((void*)appeared_mensaje1,APPEARED_POKEMON,socket1);

	//Cierro el socket utilizaado
	liberar_conexion(socket1);

	int socket2 = crear_conexion("127.0.0.2","5002");
	t_appeared_pokemon* appeared_mensaje2 = appeared_pokemon_create("messi",1,1);
	enviar_mensaje((void*)appeared_mensaje2,APPEARED_POKEMON,socket2);
	liberar_conexion(socket2);

	int socket3 = crear_conexion("127.0.0.2","5002");
	t_appeared_pokemon* appeared_mensaje3 = appeared_pokemon_create("pikachu",5,2);
	enviar_mensaje((void*)appeared_mensaje3,APPEARED_POKEMON,socket3);
	liberar_conexion(socket3);

	int socket4 = crear_conexion("127.0.0.2","5002");
	t_appeared_pokemon* appeared_mensaje4 = appeared_pokemon_create("charmander",0,4);
	enviar_mensaje((void*)appeared_mensaje4,APPEARED_POKEMON,socket4);
	liberar_conexion(socket4);

	return EXIT_SUCCESS;
}
