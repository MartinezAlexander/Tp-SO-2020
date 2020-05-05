#ifndef SERVER_H_
#define SERVER_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>

pthread_t thread;

void iniciar_servidor(char* ip, char* puerto,void (*serve_client)(int *socket));
void esperar_cliente(int socket_servidor,void (*serve_client)(int *socket));

#endif
