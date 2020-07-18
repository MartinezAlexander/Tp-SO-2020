#include "administrador_mensajes.h"
#include <commons/string.h>

void iniciar_servidor(char* ip, char* puerto,void (*serve_client)(int *socket), t_proceso proceso_servidor)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

	int activado = 1;
	setsockopt(socket_servidor,SOL_SOCKET,SO_REUSEADDR,&activado,sizeof(activado));

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    while(1)
    	esperar_cliente(socket_servidor, serve_client, proceso_servidor);
}

void esperar_cliente(int socket_servidor,void (*serve_client)(int *socket), t_proceso proceso_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int* socket_cliente = malloc(sizeof(int));
	//*socket_cliente = accept(socket_servidor, (void*) &dir_cliente, (void*)&tam_direccion);
	//loggear_conexion_al_broker(*socket_cliente);

	//pthread_create(&thread,NULL,(void*)serve_client,socket_cliente);
	//pthread_detach(thread);

	*socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	t_proceso proceso;
	int codigo = recv(*socket_cliente, &proceso, sizeof(t_proceso),MSG_WAITALL);
	if(codigo > 0){
		int se_envio = send(*socket_cliente, &proceso_servidor, sizeof(t_proceso), MSG_NOSIGNAL);
		if(se_envio > 0){
			if(recibir_ACK(*socket_cliente)){
				loggear_conexion_al_broker(*socket_cliente);
				pthread_create(&thread,NULL,(void*)serve_client,socket_cliente);
				pthread_detach(thread);
			}
		}
	}
}

void administrar_mensajes(int* socket){
	t_mensaje* mensaje = recibir_mensaje(*socket);
	if(mensaje != NULL){
		switch (mensaje->codigo) {
		case SUSCRIPCION:
			cola_suscripciones_agregar_suscripcion(mensaje,socket);
			break;
		case NEW_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_new, mensaje, &ultimo_id, socket);
			break;
		case LOCALIZED_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_localized, mensaje, &ultimo_id, socket);
			break;
		case GET_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_get, mensaje, &ultimo_id, socket);
			break;
		case APPEARED_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_appeared, mensaje, &ultimo_id, socket);
			break;
		case CATCH_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_catch, mensaje, &ultimo_id, socket);
			break;
		case CAUGHT_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_caught, mensaje, &ultimo_id, socket);
			break;
		default:
			printf("CODIGO DE MENSAJE NO VALIDO \n\n");
		}
	}
}
