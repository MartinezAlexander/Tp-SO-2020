#include "conexion_broker.h"

void iniciar_conexion_broker(){
	//Inicializamos los semaforos del procesamiento de cada cola
	//estos los utilizamos para bloquear cada hilo (que reciben los mensajes)
	//cuando tengamos que reconectarnos al broker por que se cayo la
	//conexion
	sem_init(&semaforo_procesamiento_appeared, 0, 0);
	sem_init(&semaforo_procesamiento_caught, 0, 0);
	sem_init(&semaforo_procesamiento_localized, 0, 0);
	pthread_mutex_init(&mutex_reconexion, NULL);
	estoy_reconectando = 0;

	//Realizamos la suscripcion, y posteriormente, el recibimiento y procesamiento de
	//mensajes para cada cola en hilos distintos
	pthread_create(&hilo_appeared, NULL, (void*) iniciar_suscripcion_broker,(void*)APPEARED_POKEMON);
	sleep(1);
	pthread_create(&hilo_caught, NULL, (void*) iniciar_suscripcion_broker,(void*)CAUGHT_POKEMON);
	sleep(1);
	pthread_create(&hilo_localized, NULL, (void*) iniciar_suscripcion_broker,(void*)LOCALIZED_POKEMON);

	//Iniciamos un hilo de reconexion, este comenzara bloqueado por lo que
	//no ejecutara nada, hasta que haya un problema en la conexion
	//y tengamos que reconectarnos
	sem_init(&semaforo_reconexion, 0, 0);
	pthread_create(&hilo_reconexion, NULL, (void*) reconectar_al_broker, NULL);
}

void cerrar_conexion_broker(){
	finalizo_team = 1;
	pthread_join(hilo_appeared, NULL);
	pthread_join(hilo_caught, NULL);
	pthread_join(hilo_localized, NULL);
}

//TODO: Testear bien cual es el problema aca.
//Idea que se nos habia ocurrido pero no sabemos en realidad
//es que puede que falle porque estamos liberando conexion
//muy rapido

//Posible solucion parecida a lo de la suscripcion: sleep entre cada envio
void enviar_get_objetivo(t_list* objetivo_global){

	//Por cada especie distinta a la anterior
	//(la lista esta ordenada, asi que mandare
	//un get por cada especie)
	char* ultima_especie_enviada = "";
	for(int i = 0 ; i < list_size(objetivo_global) ; i++){
		char* pokemon = list_get(objetivo_global, i);

		if(strcmp(pokemon, ultima_especie_enviada) != 0){

			sleep(1);
			//Envio mensaje GET al broker
			t_get_pokemon* mensaje_get = get_pokemon_create(pokemon);
			t_mensaje* mensaje = mensaje_simple_create(mensaje_get,GET_POKEMON);

			int socket = crear_conexion(ip_broker, puerto_broker);

			if(socket >= 0){
				int envio = enviar_mensaje(mensaje, socket);
				if(envio < 0){
					loggear_error_broker("envio de mensaje get");
				}else{
					recibir_id(socket);
				}

				liberar_conexion(socket);

				printf("Enviando especie GET al broker: %s\n", pokemon);
			}else{
				loggear_error_broker("envio de mensaje get");
			}

			ultima_especie_enviada = pokemon;
		}
	}
}

void iniciar_suscripcion_broker(op_code cola){
	switch(cola){
		case APPEARED_POKEMON:
			suscribirse_a_cola(&socket_appeared, cola);
			recibir_appeared();
			break;
		case CAUGHT_POKEMON:
			suscribirse_a_cola(&socket_caught, cola);
			recibir_caught();
			break;
		case LOCALIZED_POKEMON:
			suscribirse_a_cola(&socket_localized, cola);
			recibir_localized();
			break;
		default:
			printf("Error: Se realizo una suscripcion a una cola no correspondiente\n");
			return;
	}
}

void suscribirse_a_cola(int* socket, op_code cola){
	t_suscripcion* suscripcion = suscripcion_proceso_create(TEAM, team_id, cola);
	t_mensaje* mensaje = mensaje_simple_create((void*) suscripcion, SUSCRIPCION);

	*socket = crear_conexion(ip_broker, puerto_broker);
	//Reintento conexion
	while(*socket < 0){
		loggear_inicio_reintento_broker();
		sleep(tiempo_de_reconexion);
		*socket = crear_conexion(ip_broker, puerto_broker);
		loggear_resultado_reintento_broker(*socket > 0);
	}

	int envio = enviar_mensaje(mensaje, *socket);
	//Es necesario???
	while(envio < 0){
		loggear_inicio_reintento_broker();
		sleep(tiempo_de_reconexion);
		envio = enviar_mensaje(mensaje, *socket);
		loggear_resultado_reintento_broker(envio > 0);
	}

	int confirmacion = recibir_confirmacion_suscripcion(*socket);
	if(confirmacion == 0){
		printf("Error: el broker no acepto la suscripcion a la cola %d\n", cola);
	}

	printf("Suscripcion a la cola %d del broker exitosa\n", cola);
}

void recibir_appeared(){
	while(!finalizo_team){
		t_mensaje* mensaje = recibir_mensaje(socket_appeared);
		if(mensaje == NULL){
			iniciar_reconexion();
			sem_wait(&semaforo_procesamiento_appeared);
		}else{
			loggear_nuevo_mensaje(mensaje);
			enviar_ACK(socket_appeared);
			procesar_appeared((t_appeared_pokemon*) mensaje->mensaje);
		}
	}
	liberar_conexion(socket_appeared);
}

void recibir_caught(){
	while(!finalizo_team){
		t_mensaje* mensaje = recibir_mensaje(socket_caught);
		if(mensaje == NULL){
			iniciar_reconexion();
			sem_wait(&semaforo_procesamiento_caught);
		}else{
			loggear_nuevo_mensaje(mensaje);
			enviar_ACK(socket_caught);
			procesar_caught((t_caught_pokemon*) mensaje->mensaje, mensaje->id_correlativo);
		}
	}
	liberar_conexion(socket_caught);
}

void recibir_localized(){
	while(!finalizo_team){
		t_mensaje* mensaje = recibir_mensaje(socket_localized);
		if(mensaje == NULL){
			iniciar_reconexion();
			sem_wait(&semaforo_procesamiento_localized);
		}else{
			loggear_nuevo_mensaje(mensaje);
			enviar_ACK(socket_localized);
			procesar_localized((t_localized_pokemon*) mensaje->mensaje);
		}
	}
	liberar_conexion(socket_localized);
}

void iniciar_reconexion(){
	pthread_mutex_lock(&mutex_reconexion);
	if(!estoy_reconectando){
		estoy_reconectando = 1;
		sem_post(&semaforo_reconexion);
	}
	pthread_mutex_unlock(&mutex_reconexion);
}

//El hilo de reconexion, no lo joineamos, sino que una vez
//que termine el proceso team, el hilo va a morir
void reconectar_al_broker(){
	while(!finalizo_team){
		sem_wait(&semaforo_reconexion);
		loggear_inicio_reintento_broker();
		suscribirse_a_cola(&socket_appeared, APPEARED_POKEMON);
		sem_post(&semaforo_procesamiento_appeared);
		suscribirse_a_cola(&socket_caught, CAUGHT_POKEMON);
		sem_post(&semaforo_procesamiento_caught);
		suscribirse_a_cola(&socket_localized, LOCALIZED_POKEMON);
		sem_post(&semaforo_procesamiento_localized);
		loggear_resultado_reintento_broker(1);
		estoy_reconectando = 0;
	}
}
