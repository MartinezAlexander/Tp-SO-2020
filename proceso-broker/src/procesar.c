#include "procesar.h"

void procesar_suscripcion(t_mensaje* mensaje, int* socket,t_memoria_cache* memoria){
	t_suscripcion* suscripcion = (t_suscripcion*)mensaje->mensaje;
	t_suscriptor* suscriptor = suscriptor_create(*socket,suscripcion->pid);

	para_envio_mensaje_cacheados* parametros = parametros_create(suscriptor,suscripcion->cola_suscripcion,memoria);
	pthread_t envio_mensajes_cacheados;
	pthread_create(&envio_mensajes_cacheados,NULL,(void*)memoria_cache_enviar_mensajes_cacheados,parametros);

	t_cola_mensajeria* cola = cola_mensajeria_obtener(suscripcion->cola_suscripcion);

	int posicion_suscriptor = suscriptor_esta_suscripto(cola->suscriptores,suscriptor);

	if(posicion_suscriptor < 0){
		//TODO asegurar mutua exclusion
		suscriptor_suscribirse_a(cola->suscriptores,suscriptor);
		//puts("SUSCRIBI A: ");
	}else{
		//TODO asegurar mutua exclusion
		suscriptor_reconectar(cola->suscriptores,suscriptor,posicion_suscriptor);
		//puts("ACTUALICE EL SOCKET DE:");
	}

	if (list_size(cola->suscriptores) == 1) {
		sem_post(&cola->semaforoSuscriptores);
	}

	//TODO enviar todos los mensajes correspondientes a la cola que esten en cache hilo

	//suscripcion_proceso_mostrar(suscripcion);
}

void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje){
	int x;
	for(int i = 0; i < list_size(suscriptores); i++){
		t_suscriptor* suscriptor = list_get(suscriptores,i);
		x = enviar_mensaje(mensaje,suscriptor->socket);
		if(x > 0){
			/*printf("Enviado:\n");
			mensaje_mostrar(mensaje);*/
			char* mensaje_a_loggear = string_from_format("Enviado a %d a traves del socket %d ",suscriptor->pid,suscriptor->socket);
			string_append(&mensaje_a_loggear,mensaje_to_string(mensaje));
			log_info(loger,mensaje_a_loggear);
		}else{
			/*printf("No pudo enviarse\n");
			mensaje_mostrar(mensaje);*/
			char* mensaje_a_loggear = string_from_format("No enviado a %d a traves del socket %d ",suscriptor->pid,suscriptor->socket);
			string_append(&mensaje_a_loggear,mensaje_to_string(mensaje));
			log_info(loger,mensaje_a_loggear);
		}
		recibir_ACK(suscriptor->socket);
	}
}

void procesar_pokemon(t_cola_mensajeria* cola){
	while(1){
		sem_wait(&cola->semaforoSuscriptores);
		sem_wait(&cola->semaforoMensajes);

		//TODO hacer un queue_peek
		t_mensaje* mensaje = (t_mensaje*) queue_pop(cola->queue);

		//printf("saco de la cola a: \n");
		envio_a_suscriptores(cola->suscriptores, mensaje);

		memoria_cache_agregar_mensaje(memoria,mensaje);

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

//TODO borrar al encontrar una mejor manera
void obtener_cache(t_memoria_cache* memoria_cache){
	memoria = memoria_cache;
}

void obtener_logger(t_log* logger){
	loger = logger;
}
