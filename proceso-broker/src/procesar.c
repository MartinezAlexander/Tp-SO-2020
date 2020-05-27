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

void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje, t_estado_mensaje* estado){
	int x;
	for(int i = 0; i < list_size(suscriptores); i++){
		t_suscriptor* suscriptor = list_get(suscriptores,i);
		printf("PID: %d\n",suscriptor->pid);
		x = enviar_mensaje(mensaje,suscriptor->socket);
		printf("envio de mensaje: %d\n\n",x);
		estado->id_mensaje = mensaje->id;
		if(x>0){
			list_add(estado->enviados,(void*)suscriptor->pid);
		}else{
			list_add(estado->fallidos,(void*)suscriptor->pid);
		}
	}
}

void procesar_pokemon(t_cola_mensajeria* cola){
	while(1){
		sem_wait(&cola->semaforoSuscriptores);
		sem_wait(&cola->semaforoMensajes);

		t_mensaje* pokemon = (t_mensaje*) queue_pop(cola->queue);
		t_estado_mensaje* estado = estado_mensaje_create(pokemon->id);
		//guardar pokemon en la cache
		printf("saco de la cola a: \n");
		envio_a_suscriptores(cola->suscriptores, pokemon, estado);
		printf("cantidad de enviados %d\n", list_size(estado->enviados));
		printf("cantidad de fallidos %d\n", list_size(estado->fallidos));

/*
		while(!list_is_empty(estado->fallidos)){
			reintentar_fallidos(estado->fallidos,);
		}
*/
		//caught_pokemon_mostrar((t_caught_pokemon*)pokemon->mensaje);

		sem_post(&cola->semaforoSuscriptores);
	}
}

t_estado_mensaje* estado_mensaje_create(int32_t id){
	t_estado_mensaje* estado = malloc(sizeof(t_estado_mensaje));
	estado->id_mensaje = id;
	estado->enviados = list_create();
	estado->fallidos = list_create();
	return estado;
}

void estado_mensaje_destroy(t_estado_mensaje* estado){
	list_destroy_and_destroy_elements(estado->enviados,free);
	list_destroy_and_destroy_elements(estado->fallidos,free);
	free(estado);
}
