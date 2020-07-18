#include "conexion_gameboy.h"

void iniciar_puerto_de_escucha(){

	iniciar_servidor(ip_gamecard, puerto_gamecard,(void*) procesar_mensajes_directos, GAMECARD);
	printf("[Gameboy] Iniciando puerto de escucha directo al gameboy\n");
}


void procesar_mensajes_directos(int* socket){
	//TODO LIBERAR SOCKET CUANDO CAMBIE LA BIBLIOTECA
	t_mensaje* mensaje = recibir_mensaje(*socket);

	if(mensaje!=NULL){
		enviar_ACK(*socket);

		switch(mensaje->codigo){
			case NEW_POKEMON:
				procesar_mensaje(mensaje, ejecutar_new);
				break;
			case CATCH_POKEMON:
				procesar_mensaje(mensaje, ejecutar_catch);
				break;
			case GET_POKEMON:
				procesar_mensaje(mensaje, ejecutar_get);
				break;
			default:
				printf("\n--Mensaje directo recibido no procesable--\n");
				break;
		}
	}
}
