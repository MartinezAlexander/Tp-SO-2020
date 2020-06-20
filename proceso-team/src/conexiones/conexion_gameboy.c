#include "conexion_gameboy.h"

void iniciar_puerto_de_escucha(){
	iniciar_servidor(ip_team, puerto_team,(void*) procesar_mensajes_directos);
	printf("[Gameboy] Iniciando puerto de escucha directo al gameboy\n");
}

//Para procesar los mensajes usamos las mismas funciones de procesamiento
//que para el broker.
//Supuestamente solo puedo recibir un APPEARED del gameboy, pero como ya
//tenemos las funciones hechas dejamos los otros dos casos tambien, por las dudas
void procesar_mensajes_directos(int* socket){
	t_mensaje* mensaje = recibir_mensaje(*socket);
	loggear_nuevo_mensaje(mensaje);
	enviar_ACK(*socket);

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
