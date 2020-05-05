#ifndef CLIENTE_H_
#define CLIENTE_H_

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include <string.h>

int crear_conexion(char* ip, char* puerto);
void liberar_conexion(int socket_cliente);

#endif
