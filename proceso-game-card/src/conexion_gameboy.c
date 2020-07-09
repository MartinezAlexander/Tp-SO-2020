#include "conexion_gameboy.h"

void iniciar_puerto_de_escucha(){
	//TODO agregar en config puerto y servidor de gamecard
	iniciar_servidor("127.0.0.3", "5001",(void*) procesar_mensajes_directos);
	printf("[Gameboy] Iniciando puerto de escucha directo al gameboy\n");
}

//Para procesar los mensajes usamos las mismas funciones de procesamiento
//que para el broker.
//Supuestamente solo puedo recibir un APPEARED del gameboy, pero como ya
//tenemos las funciones hechas dejamos los otros dos casos tambien, por las dudas
void procesar_mensajes_directos(int* socket){
	t_mensaje* mensaje = recibir_mensaje(*socket);

	//loggear_nuevo_mensaje(mensaje);

	if(mensaje!=NULL){
		enviar_ACK(*socket);

		switch(mensaje->codigo){
			case NEW_POKEMON:
				ejecutar_new((t_new_pokemon*)mensaje->mensaje,mensaje->id);
				break;
			case CATCH_POKEMON:
				ejecutar_catch((t_catch_pokemon*)mensaje->mensaje, mensaje->id);
				break;
			case GET_POKEMON:
				ejecutar_get((t_get_pokemon*)mensaje->mensaje, mensaje->id);
				break;
			default:
				printf("\n--Mensaje directo recibido no procesable--\n");
				break;
		}
	}
}
