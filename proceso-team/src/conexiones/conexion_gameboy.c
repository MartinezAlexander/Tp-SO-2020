#include "conexion_gameboy.h"

void iniciar_puerto_de_escucha(){
	iniciar_servidor(ip_team, puerto_team,(void*) procesar_mensajes_directos);
}

void procesar_mensajes_directos(int* socket){
	t_mensaje* mensaje = recibir_mensaje(*socket);
	loggear_nuevo_mensaje(mensaje);

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
