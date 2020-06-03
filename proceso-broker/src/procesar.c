#include "procesar.h"
#include <unistd.h>

void procesar_suscripcion(t_mensaje* mensaje, int* socket,t_memoria_cache* memoria){
	t_suscripcion* suscripcion = (t_suscripcion*)mensaje->mensaje;
	t_suscriptor* suscriptor = suscriptor_create(*socket,suscripcion->pid);

	loggear_recepcion_mensaje(mensaje_to_string(mensaje));

	confirmar_suscripcion(*socket);

	para_envio_mensaje_cacheados* parametros = parametros_create(suscriptor,suscripcion->cola_suscripcion,memoria);
	pthread_t envio_mensajes_cacheados;
	pthread_create(&envio_mensajes_cacheados,NULL,(void*)memoria_cache_enviar_mensajes_cacheados,parametros);

	t_cola_mensajeria* cola = cola_mensajeria_obtener(suscripcion->cola_suscripcion);

	int posicion_suscriptor = suscriptor_esta_suscripto(cola->suscriptores,suscriptor);

	if(posicion_suscriptor < 0){
		//TODO asegurar mutua exclusion
		suscriptor_suscribirse_a(cola->suscriptores,suscriptor);

		loggear_suscripcion_proceso(suscripcion_proceso_to_string(suscripcion));

	}else{
		//TODO asegurar mutua exclusion
		suscriptor_reconectar(cola->suscriptores,suscriptor,posicion_suscriptor);

		//TODO pasar a log personal
		loggear_reconexion_proceso(suscripcion_proceso_to_string(suscripcion));
	}

	//TODO dudoso, turbio, probar en maquina de ale
	pthread_join(envio_mensajes_cacheados,NULL);

	if (list_size(cola->suscriptores) == 1) {
		sem_post(&cola->semaforoSuscriptores);
	}
}

void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje){
	int x;
	for(int i = 0; i < list_size(suscriptores); i++){
		t_suscriptor* suscriptor = list_get(suscriptores,i);
		x = enviar_mensaje(mensaje,suscriptor->socket);
		if(x > 0){
			loggear_envio_mensaje(mensaje_to_string(mensaje));
			//TODO verificar el retorno del ACK
			recibir_ACK(suscriptor->socket);
			loggear_recepcion_ACK(suscriptor_to_string(suscriptor));
		}else{
			//TODO pasar a log personal
			log_personal_error_envio_a_suscriptor(suscriptor_to_string(suscriptor));
		}
	}
}

void procesar_pokemon(t_cola_mensajeria* cola){
	while(1){
		sem_wait(&cola->semaforoSuscriptores);
		sem_wait(&cola->semaforoMensajes);

		//TODO hacer un queue_peek
		t_mensaje* mensaje = (t_mensaje*) queue_pop(cola->queue);

		envio_a_suscriptores(cola->suscriptores, mensaje);

		memoria_cache_agregar_mensaje(memoria_cache,mensaje);

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
