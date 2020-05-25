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

	if (list_size(cola->suscriptores) == 1) {
		sem_post(&cola->semaforoSuscriptores);
	}

	suscripcion_proceso_mostrar(suscripcion);
}

void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje){
	for(int i = 0; i < list_size(suscriptores); i++){
		t_suscriptor* suscriptor = list_get(suscriptores,i);
		enviar_mensaje(mensaje,suscriptor->socket);
	}
}

void procesar_pokemon(t_cola_mensajeria* cola){
	while(1){
		sem_wait(&cola->semaforoSuscriptores);
		sem_wait(&cola->semaforoMensajes);

		t_mensaje* pokemon = (t_mensaje*) queue_pop(cola->queue);
		//guardar pokemon en la cache
		printf("saco de la cola a: \n");
		envio_a_suscriptores(cola->suscriptores, pokemon);
		//caught_pokemon_mostrar((t_caught_pokemon*)pokemon->mensaje);

		sem_post(&cola->semaforoSuscriptores);
	}
}
