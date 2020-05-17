#include "conexion_gameboy.h"

void iniciar_puerto_de_escucha(){
	//TODO definir ip y puerto del server por config por ahi??
	char* ip = "127.0.0.2";
	char* puerto = "5002";
	iniciar_servidor(ip, puerto,(void*) procesar_mensajes_directos);
}

void procesar_mensajes_directos(int* socket){
	t_mensaje* mensaje = recibir_mensaje(*socket);

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
