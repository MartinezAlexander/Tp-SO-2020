#include "procesar.h"

void procesar_suscripcion(t_mensaje* mensaje, int* socket){
	t_suscripcion* suscripcion = (t_suscripcion*)mensaje->mensaje;
	t_suscriptor* suscriptor = suscriptor_create(*socket,suscripcion->pid);
	t_cola_mensajeria* cola = cola_mensajeria_obtener(suscripcion->cola_suscripcion);
	int posicion_suscriptor = suscriptor_esta_suscripto(cola->suscriptores,suscriptor);
	if(posicion_suscriptor < 0){
		suscriptor_suscribirse_a(cola->suscriptores,suscriptor);
		puts("SUSCRIBI A: ");
	}else{
		suscriptor_reconectar(cola->suscriptores,suscriptor,posicion_suscriptor);
		puts("ACTUALICE EL SOCKET DE:");
	}
	suscripcion_proceso_mostrar(suscripcion);
}

void procesar_pokemon(t_cola_mensajeria* cola){
	while(1){
		if(!queue_is_empty(cola->queue)){
			if(!list_is_empty(cola->suscriptores)){
				t_mensaje* pokemon = (t_mensaje*)queue_pop(cola->queue);
				//guardar pokemon en la cache
				printf("saco de la cola a: \n");
				envio_a_suscriptores(cola->suscriptores,pokemon);
				//caught_pokemon_mostrar((t_caught_pokemon*)pokemon->mensaje);
			}
		}
	}
}
