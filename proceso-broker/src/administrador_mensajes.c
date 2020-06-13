#include "administrador_mensajes.h"
#include <commons/string.h>

void iniciar_servidor(char* ip, char* puerto,void (*serve_client)(int *socket))
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
    	esperar_cliente(socket_servidor, serve_client);
}

void esperar_cliente(int socket_servidor,void (*serve_client)(int *socket))
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);
	loggear_conexion_al_broker(socket_cliente);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);

}

void administrar_mensajes(int* socket){
	//TODO Syscall param socketcall.recv(args) points to uninitialised byte(s)
	t_mensaje* mensaje = recibir_mensaje(*socket);
	switch(mensaje->codigo){
		case SUSCRIPCION:
			procesar_suscripcion(mensaje, socket);
			break;
		case NEW_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_new,mensaje,&ultimo_id);
			//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
			enviar_id(*socket,ultimo_id);
			break;
		case LOCALIZED_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_localized,mensaje,&ultimo_id);
			//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
			enviar_id(*socket,ultimo_id);
			break;
		case GET_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_get,mensaje,&ultimo_id);
			//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
			enviar_id(*socket,ultimo_id);
			break;
		case APPEARED_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_appeared,mensaje,&ultimo_id);
			//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
			enviar_id(*socket,ultimo_id);
			break;
		case CATCH_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_catch,mensaje,&ultimo_id);
			//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
			enviar_id(*socket,ultimo_id);
			break;
		case CAUGHT_POKEMON:
			cola_mensajeria_recibir_mensaje(cola_mensajeria_caught,mensaje,&ultimo_id);
			//TODO Syscall param socketcall.send(args) points to uninitialised byte(s)
			enviar_id(*socket,ultimo_id);
			break;
		default:
			printf("CODIGO DE MENSAJE NO VALIDO \n\n");
	}
}
