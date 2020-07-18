#include "conexion_broker.h"

void iniciar_conexion_broker(){
	/*
	 * Primero, inicializo los semaforos de cada hilo de procesamiento.
	 * Basicamente quiero poder frenar los hilos que reciben y procesan
	 * cada mensaje en caso de que tenga que reconectarme al broker, para
	 * eso utilizo estos semaforos
	 */
	sem_init(&semaforo_procesamiento_catch, 0, 0);
	sem_init(&semaforo_procesamiento_get, 0, 0);
	sem_init(&semaforo_procesamiento_new, 0, 0);
	/*
	 * Luego, inicio los hilos para procesar cada mensaje.
	 * Cada hilo lo que va a hacer es suscribirse a su cola del broker
	 * y luego quedara en un loop recibiendo mensajes y procesandolos
	*/
	//TODO posible creacion de pthread_detach
	pthread_create(&hilo_catch, NULL, (void*)procesar_cola, (void*)CATCH_POKEMON);
	sleep(1);
	//TODO posible creacion de pthread_detach
	pthread_create(&hilo_get, NULL, (void*)procesar_cola, (void*)GET_POKEMON);
	sleep(1);
	//TODO posible creacion de pthread_detach
	pthread_create(&hilo_new, NULL, (void*)procesar_cola, (void*)NEW_POKEMON);
	/*
	 * Finalmente, inicializo el hilo de reconexion. Este empezara bloqueado
	 * por su semaforo, y se activara cuando detecte que se corto la conexion
	 * con el broker. Llevara a cabo la reconexion, y desbloqueara a los hilos
	 * de procesamiento para que puedan seguir recibiendo mensajes.
	*/
	sem_init(&semaforo_reconexion, 0, 0);
	//TODO Crear pthread_detach
	pthread_create(&hilo_reconexion, NULL, (void*)reconectar, NULL);
	/*
	 *  Ademas, necesito iniciar un mutex que servira para que los 3 hilos no manden
	 * la señal de activacion al hilo de reconexion 3 veces.
	 */
	pthread_mutex_init(&mutex_reconexion, NULL);
	estoy_reconectando = 0;
}

void procesar_cola(op_code cola){
	switch(cola){
		case CATCH_POKEMON:
			suscribirse_a_cola(&socket_catch, cola);
			procesar_catch();
			break;
		case GET_POKEMON:
			suscribirse_a_cola(&socket_get, cola);
			procesar_get();
			break;
		case NEW_POKEMON:
			suscribirse_a_cola(&socket_new, cola);
			procesar_new();
			break;
		default:
			printf("Error: Se realizo una suscripcion a una cola no correspondiente\n");
			return;
	}
}

void suscribirse_a_cola(int* socket, op_code cola){
	t_suscripcion* suscripcion = suscripcion_proceso_create(GAMECARD, card_id, cola);
	t_mensaje* mensaje = mensaje_simple_create((void*) suscripcion, SUSCRIPCION);

	*socket = crear_conexion(ip_broker, puerto_broker);

	int conexion_exitosa = handshake(GAMECARD, BROKER, *socket);

	//Reintento conexion
	while(conexion_exitosa == 0){
		sleep(tiempo_reintento_conexion);
		*socket = crear_conexion(ip_broker, puerto_broker);

		conexion_exitosa = handshake(GAMECARD, BROKER, *socket);
	}

	int envio = enviar_mensaje(mensaje, *socket);
	while(envio < 0){
		sleep(tiempo_reintento_conexion);
		envio = enviar_mensaje(mensaje, *socket);
	}

	int confirmacion = recibir_confirmacion_suscripcion(*socket);
	if(confirmacion == 0){
		printf("Error: el broker no acepto la suscripcion a la cola %d\n", cola);
	}

	printf("Suscripcion a la cola %d del broker exitosa\n", cola);
}

void procesar_catch(){
	while(1){
		t_mensaje* mensaje = recibir_mensaje(socket_catch);
		if(mensaje == NULL){
			iniciar_reconexion();
			sem_wait(&semaforo_procesamiento_catch);
		}else{
			enviar_ACK(socket_catch);
			procesar_mensaje(mensaje, ejecutar_catch);
		}
	}
	liberar_conexion(socket_catch);
}

void procesar_get(){
	while(1){
		t_mensaje* mensaje = recibir_mensaje(socket_get);
		if(mensaje == NULL){
			iniciar_reconexion();
			sem_wait(&semaforo_procesamiento_get);
		}else{
			enviar_ACK(socket_get);
			procesar_mensaje(mensaje, ejecutar_get);
		}
	}
	liberar_conexion(socket_get);
}

void procesar_new(){
	while(1){
		t_mensaje* mensaje = recibir_mensaje(socket_new);
		if(mensaje == NULL){
			iniciar_reconexion();
			sem_wait(&semaforo_procesamiento_new);
		}else{
			enviar_ACK(socket_new);
			procesar_mensaje(mensaje, ejecutar_new);
		}
	}
	liberar_conexion(socket_new);
}

/*
 * Esta funcion la debo llamar cuando detecto que se cayo el broker
 * y quiero empezar a reconectarme. La idea es que como tranquilamente
 * puede pasar que los 3 hilos detecten que se cayo al mismo tiempo
 * rodearlo con un mutex, y controlar a traves de una variable si debo
 * mandar la señal al hilo de reconexion o no, para que esto se haga una
 * vez y no mande 3 signal al mismo tiempo.
 */
void iniciar_reconexion(){
	pthread_mutex_lock(&mutex_reconexion);
	if(!estoy_reconectando){
		estoy_reconectando = 1;
		sem_post(&semaforo_reconexion);
	}
	pthread_mutex_unlock(&mutex_reconexion);
}

void reconectar(){
	while(1){
		sem_wait(&semaforo_reconexion);
		suscribirse_a_cola(&socket_catch, CATCH_POKEMON);
		sem_post(&semaforo_procesamiento_catch);
		suscribirse_a_cola(&socket_get, GET_POKEMON);
		sem_post(&semaforo_procesamiento_get);
		suscribirse_a_cola(&socket_new, NEW_POKEMON);
		sem_post(&semaforo_procesamiento_new);
		estoy_reconectando = 0;
	}
}
