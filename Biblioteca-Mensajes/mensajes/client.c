#include "client.h"

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		socket_cliente = -1;

	freeaddrinfo(server_info);

	return socket_cliente;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

int handshake(t_proceso yo, t_proceso otro, int socket){
	int conexion_exitosa = 0;
	t_proceso proceso = yo;
	int codigo = send(socket, &proceso, sizeof(t_proceso),MSG_NOSIGNAL);
	if(codigo > 0){
		t_proceso proceso_socket;
		int se_recibio = recv(socket, &proceso_socket, sizeof(t_proceso), MSG_WAITALL);
		if(se_recibio > 0){
			if(proceso_socket == otro){
				int ack = enviar_ACK(socket);
				if (ack > 0){
					conexion_exitosa = 1;
				}
			}
		}
	}
	return conexion_exitosa;
}
