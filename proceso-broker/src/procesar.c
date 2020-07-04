#include "procesar.h"
#include <unistd.h>

void procesar_suscripcion(){

	while(1){
		sem_wait(&cola_suscripciones->semaforoSuscripcion);

		pthread_mutex_lock(&cola_suscripciones->mutex_cola_suscripcion);
		t_suscripciones* info_suscripcion = queue_pop(cola_suscripciones->queue);
		pthread_mutex_unlock(&cola_suscripciones->mutex_cola_suscripcion);

		t_suscripcion* suscripcion =(t_suscripcion*) info_suscripcion->mensaje->mensaje;

		t_cola_mensajeria* cola = cola_mensajeria_obtener(suscripcion->cola_suscripcion);

		int posicion_suscriptor = suscriptor_esta_suscripto(cola->suscriptores,suscripcion->pid);

		if (posicion_suscriptor < 0) {
			//TODO helgrind socket 2
			t_suscriptor* suscriptor = suscriptor_create(*info_suscripcion->socket,suscripcion->pid);
			memoria_cache_enviar_mensajes_cacheados(*info_suscripcion->socket,suscriptor->mensajes_recibidos,suscriptor->pid,suscripcion->cola_suscripcion);
			pthread_mutex_lock(&cola->semaforoSuscriptores);
			suscriptor_suscribirse_a(cola->suscriptores, suscriptor);
			pthread_mutex_unlock(&cola->semaforoSuscriptores);

			loggear_suscripcion_proceso(suscripcion_proceso_to_string(suscripcion));

		} else {
			pthread_mutex_lock(&cola->semaforoSuscriptores);
			t_suscriptor* suscriptor = list_get(cola->suscriptores,posicion_suscriptor);
			//TODO helgrind socket 4
			memoria_cache_enviar_mensajes_cacheados(*info_suscripcion->socket,suscriptor->mensajes_recibidos,suscriptor->pid,suscripcion->cola_suscripcion);
			suscriptor_reconectar(cola->suscriptores, info_suscripcion->socket,posicion_suscriptor);
			pthread_mutex_unlock(&cola->semaforoSuscriptores);
			loggear_reconexion_proceso(suscripcion_proceso_to_string(suscripcion));
		}

		suscripciones_destroy(info_suscripcion);
	}
}

void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje){
	int x;
	for(int i = 0; i < list_size(suscriptores); i++){
		t_suscriptor* suscriptor = list_get(suscriptores,i);

		if(!suscriptor_ya_recibio_mensaje(suscriptor->mensajes_recibidos,mensaje->id)){
			//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
			x = enviar_mensaje(mensaje, suscriptor->socket);

			if (x > 0) {
				loggear_envio_mensaje(mensaje_to_string(mensaje));
				//TODO Syscall param socketcall.recv(args) points to uninitialised byte(s)
				if(recibir_ACK(suscriptor->socket)){
					//TODO Use of uninitialised value of size 4
					//TODO char* suscriptor = suscriptor_to_string(suscriptor);
					loggear_recepcion_ACK(suscriptor_to_string(suscriptor));
					suscriptor_agregar_mensaje_recibido(suscriptor->mensajes_recibidos,mensaje->id);
				}else{
					//TODO Loguear que no se recibio el ack en loguer personal
				}

			} else {
				log_personal_error_envio_a_suscriptor(suscriptor_to_string(suscriptor));
			}
		}else{
			//TODO loguear que el mensaje ya ha sido enviado al suscriptor
			puts("suscriptor ya recibio el mensaje");
		}
	}
}

void procesar_pokemon(t_cola_mensajeria* cola){
	while(1){
		sem_wait(&cola->semaforoMensajes);

		pthread_mutex_lock(&cola->mutex_cola_mensaje);
		t_mensaje* mensaje = (t_mensaje*) queue_pop(cola->queue);
		pthread_mutex_unlock(&cola->mutex_cola_mensaje);

		if(string_equals_ignore_case(algoritmo_memoria,"BS")){
			cachear_mensaje_bs(mensaje);
		}else{
			administrador_cachear_mensaje(mensaje);
		}

		pthread_mutex_lock(&cola->semaforoSuscriptores);
		int hay_suscriptores = !list_is_empty(cola->suscriptores);
		pthread_mutex_unlock(&cola->semaforoSuscriptores);
		if(hay_suscriptores){
			envio_a_suscriptores(cola->suscriptores, mensaje);
		}

		mensaje_destroy(mensaje);
	}
}
