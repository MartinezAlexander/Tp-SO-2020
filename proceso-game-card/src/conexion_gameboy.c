#include "conexion_gameboy.h"

void iniciar_puerto_de_escucha(){

	iniciar_servidor(ip_gamecard, puerto_gamecard,(void*) procesar_mensajes_directos);
	printf("[Gameboy] Iniciando puerto de escucha directo al gameboy\n");
}


void procesar_mensajes_directos(int* socket){
	t_mensaje* mensaje = recibir_mensaje(*socket);
	//Syscall param socketcall.recv(args) points to uninitialised byte(s)
	//133 (48 direct, 85 indirect) bytes in 3 blocks are definitely lost

	//loggear_nuevo_mensaje(mensaje);

	if(mensaje!=NULL){
		enviar_ACK(*socket);

		//Syscall param socketcall.send(args) points to uninitialised byte(s)

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
