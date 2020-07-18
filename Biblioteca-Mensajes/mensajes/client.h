#ifndef CLIENTE_H_
#define CLIENTE_H_

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include <string.h>
#include "mensajes.h"

/*
crear_conexion crea un socket cliente a un servidor que se encuentra en
el ip y puerto indicado por parametro, si logra conectarse devuelve el
valor del socket, sino devuelve -1
*/
int crear_conexion(char* ip, char* puerto);

/*
liberar_conexion destruye el socket creado
*/
void liberar_conexion(int socket_cliente);

int handshake(t_proceso yo, t_proceso otro, int socket);

#endif
