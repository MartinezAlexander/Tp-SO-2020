#include "procesar.h"
#include <unistd.h>

void procesar_suscripcion(t_mensaje* mensaje, int* socket){
	t_suscripcion* suscripcion = (t_suscripcion*)mensaje->mensaje;
	t_suscriptor* suscriptor = suscriptor_create(*socket,suscripcion->pid);

	loggear_recepcion_mensaje(mensaje_to_string(mensaje));
	//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
	confirmar_suscripcion(*socket);

	para_envio_mensaje_cacheados* parametros = parametros_create(suscriptor,suscripcion->cola_suscripcion,memoria_cache);
	pthread_t envio_mensajes_cacheados;
	pthread_create(&envio_mensajes_cacheados,NULL,(void*)memoria_cache_enviar_mensajes_cacheados,parametros);

	t_cola_mensajeria* cola = cola_mensajeria_obtener(suscripcion->cola_suscripcion);

	int posicion_suscriptor = suscriptor_esta_suscripto(cola->suscriptores,suscriptor);

	if(posicion_suscriptor < 0){
		//TODO asegurar mutua exclusion
		pthread_mutex_lock(&cola->semaforoSuscriptores);
		suscriptor_suscribirse_a(cola->suscriptores,suscriptor);
		pthread_mutex_unlock(&cola->semaforoSuscriptores);

		loggear_suscripcion_proceso(suscripcion_proceso_to_string(suscripcion));

	}else{
		//TODO asegurar mutua exclusion
		pthread_mutex_lock(&cola->semaforoSuscriptores);
		suscriptor_reconectar(cola->suscriptores,suscriptor,posicion_suscriptor);
		pthread_mutex_unlock(&cola->semaforoSuscriptores);
		loggear_reconexion_proceso(suscripcion_proceso_to_string(suscripcion));
	}

	//TODO dudoso, turbio, probar en maquina de ale
	pthread_join(envio_mensajes_cacheados,NULL);

	/*if (list_size(cola->suscriptores) == 1) {
		sem_post(&cola->semaforoSuscriptores);
	}*/

	mensaje_destroy(mensaje);
}

void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje){
	int x;
	for(int i = 0; i < list_size(suscriptores); i++){
		t_suscriptor* suscriptor = list_get(suscriptores,i);
		//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
		x = enviar_mensaje(mensaje,suscriptor->socket);
		if(x > 0){
			loggear_envio_mensaje(mensaje_to_string(mensaje));
			//TODO verificar el retorno del ACK
			//TODO Syscall param socketcall.recv(args) points to uninitialised byte(s)
			recibir_ACK(suscriptor->socket);
			//Use of uninitialised value of size 4
			//TODO char* suscriptor = suscriptor_to_string(suscriptor);
			loggear_recepcion_ACK(suscriptor_to_string(suscriptor));
		}else{
			log_personal_error_envio_a_suscriptor(suscriptor_to_string(suscriptor));
		}
	}
}

void procesar_pokemon(t_cola_mensajeria* cola){
	while(1){
		//sem_wait(&cola->semaforoSuscriptores);
		sem_wait(&cola->semaforoMensajes);

		//TODO hacer un queue_peek
		pthread_mutex_lock(&cola->mutex_cola_mensaje);
		t_mensaje* mensaje = (t_mensaje*) queue_pop(cola->queue);
		pthread_mutex_unlock(&cola->mutex_cola_mensaje);

		//memoria_cache_agregar_mensaje(memoria_cache,mensaje);
		if(string_equals_ignore_case(algoritmo_memoria,"BS")){
			cachear_mensaje_bs(mensaje);
		}else{
			administrador_cachear_mensaje(mensaje);
		}

		pthread_mutex_lock(&cola->semaforoSuscriptores);
		int no_hay_suscriptores = !list_is_empty(cola->suscriptores);
		pthread_mutex_unlock(&cola->semaforoSuscriptores);
		if(no_hay_suscriptores){
			envio_a_suscriptores(cola->suscriptores, mensaje);
		}
		mensaje_destroy(mensaje);
		//sem_post(&cola->semaforoSuscriptores);
	}
}
