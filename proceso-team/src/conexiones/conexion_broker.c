#include "conexion_broker.h"

void iniciar_conexion_broker(){
	pthread_create(&hilo_appeared, NULL, (void*) iniciar_suscripcion_broker, APPEARED_POKEMON);
	sleep(1);
	pthread_create(&hilo_caught, NULL, (void*) iniciar_suscripcion_broker, CAUGHT_POKEMON);
	sleep(1);
	pthread_create(&hilo_localized, NULL, (void*) iniciar_suscripcion_broker, LOCALIZED_POKEMON);
}

void cerrar_conexion_broker(){
	finalizo_team = 1;
	pthread_join(hilo_appeared, NULL);
	pthread_join(hilo_caught, NULL);
	pthread_join(hilo_localized, NULL);
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
	//TODO mandar team_id en vez de pid()
	t_suscripcion* suscripcion = suscripcion_proceso_create(TEAM, getpid(), cola);
	t_mensaje* mensaje = mensaje_simple_create((void*) suscripcion, SUSCRIPCION);

	int socket = crear_conexion(ip_broker, puerto_broker);
	//Reintento conexion
	while(socket < 0){
		sleep(tiempo_de_reconexion);
		socket = crear_conexion(ip_broker, puerto_broker);
	}

	enviar_mensaje(mensaje, socket);
	//Despues puedo recibir la confirmacion
	//Por ahi podria reintentar si falla.
	recibir_confirmacion_suscripcion(socket);

	switch(cola){
		case APPEARED_POKEMON:
			recibir_mensaje_appeared(socket);
			break;
		case CAUGHT_POKEMON:
			recibir_mensaje_caught(socket);
			break;
		case LOCALIZED_POKEMON:
			recibir_mensaje_localized(socket);
			break;
	}
}


//TODO ver: en caso de fallo, el broker intenta mandar de nuevo???
void recibir_mensaje_appeared(int socket_appeared){
	while(!finalizo_team){
		//Conexion esta viva? => reconecto
		t_mensaje* mensaje_appeared = recibir_mensaje(socket_appeared);
		loggear_nuevo_mensaje(mensaje_appeared);
		enviar_ACK(socket_appeared);
		procesar_appeared((t_appeared_pokemon*) mensaje_appeared->mensaje);
	}
	liberar_conexion(socket_appeared);
}

//TODO comportamiento default:
//En caso de que falle el envio de mensaje CATCH,
//de ahi en adelante tomare como resultado positivo
//cada CATCH, por loq ue puedo terminar este hilo
//y liberar esta conexion
void recibir_mensaje_caught(int socket_caught){
	while(!finalizo_team){

		t_mensaje* mensaje_caught = recibir_mensaje(socket_caught);
		loggear_nuevo_mensaje(mensaje_caught);
		enviar_ACK(socket_caught);
		procesar_caught((t_caught_pokemon*) mensaje_caught->mensaje, mensaje_caught->id_correlativo);
	}
	liberar_conexion(socket_caught);
}

//TODO comportamiento default:
//En caso de que falle el envio de mensaje GET,
//ya no voy a esperar recibibr localized, por lo que
//puedo terminar el hilo y liberar la conexion
void recibir_mensaje_localized(int socket_localized){
	while(!finalizo_team){
		t_mensaje* mensaje_localized = recibir_mensaje(socket_localized);
		loggear_nuevo_mensaje(mensaje_localized);
		enviar_ACK(socket_localized);
		procesar_localized((t_localized_pokemon*) mensaje_localized->mensaje);
	}
	liberar_conexion(socket_localized);
}
