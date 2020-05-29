#include "conexion_broker.h"

void iniciar_conexion_broker(){
	pthread_t hilo_suscripcion_appeared;
	pthread_create(&hilo_suscripcion_appeared, NULL, (void*) iniciar_suscripcion_broker, APPEARED_POKEMON);

	pthread_t hilo_suscripcion_caught;
	pthread_create(&hilo_suscripcion_caught, NULL, (void*) iniciar_suscripcion_broker, CAUGHT_POKEMON);

	pthread_t hilo_suscripcion_localized;
	pthread_create(&hilo_suscripcion_localized, NULL, (void*) iniciar_suscripcion_broker, LOCALIZED_POKEMON);
}

void enviarACK(int socket){
	enviar_confirmacion(1, ACK, socket);
}

void enviar_get_objetivo(t_list* objetivo_global){

	char* ultima_especie_enviada = "";
	for(int i = 0 ; i < list_size(objetivo_global) ; i++){
		char* pokemon = list_get(objetivo_global, i);

		if(strcmp(pokemon, ultima_especie_enviada) != 0){

			//Envio mensaje GET al broker
			t_get_pokemon* mensaje_get = get_pokemon_create(pokemon);
			t_mensaje* mensaje = mensaje_simple_create(mensaje_get,GET_POKEMON);

			int socket = crear_conexion(ip_broker, puerto_broker);
			enviar_mensaje(mensaje, socket);

			//cod_confirmacion codigo;
			//int id = recibir_confirmacion(socket, &codigo);
			liberar_conexion(socket);

			printf("Enviando especie GET al broker: %s\n", pokemon);

			ultima_especie_enviada = pokemon;
		}
	}
}


void iniciar_suscripcion_broker(op_code cola){
	t_suscripcion* suscripcion = suscripcion_proceso_create(TEAM, getpid(), cola);
	t_mensaje* mensaje = mensaje_simple_create((void*) suscripcion, SUSCRIPCION);

	int socket = crear_conexion(ip_broker, puerto_broker);

	//Reintento conexion
	while(socket < 0){
		sleep(tiempo_de_reconexion);
		socket = crear_conexion(ip_broker, puerto_broker);
	}

	enviar_mensaje(mensaje, socket);

	void* funcion_de_escucha;

	switch(cola){
		case APPEARED_POKEMON:
			funcion_de_escucha = recibir_mensaje_appeared;
			break;
		case CAUGHT_POKEMON:
			funcion_de_escucha = recibir_mensaje_caught;
			break;
		case LOCALIZED_POKEMON:
			funcion_de_escucha = recibir_mensaje_localized;
			break;
	}

	pthread_t hilo_escucha;
	pthread_create(&hilo_escucha, NULL, funcion_de_escucha, &socket);
}

void recibir_mensaje_appeared(int socket_appeared){
	while(1){
		t_mensaje* mensaje_appeared = recibir_mensaje(socket_appeared);
		loggear_nuevo_mensaje(mensaje_appeared);
		enviarACK(socket_appeared);
		procesar_appeared((t_appeared_pokemon*) mensaje_appeared->mensaje);
	}
}

void recibir_mensaje_caught(int socket_caught){
	while(1){
		t_mensaje* mensaje_caught = recibir_mensaje(socket_caught);
		loggear_nuevo_mensaje(mensaje_caught);
		enviarACK(socket_caught);
		procesar_caught((t_caught_pokemon*) mensaje_caught->mensaje, mensaje_caught->id_correlativo);
	}
}

void recibir_mensaje_localized(int socket_localized){
	while(1){
		t_mensaje* mensaje_localized = recibir_mensaje(socket_localized);
		loggear_nuevo_mensaje(mensaje_localized);
		enviarACK(socket_localized);
		procesar_localized((t_localized_pokemon*) mensaje_localized->mensaje);
	}
}
