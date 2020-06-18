#include "suscriptor.h"
#include <commons/string.h>
#include <stdio.h>
t_suscriptor* suscriptor_create(int socket, int32_t pid){
	t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
	suscriptor->pid = pid;
	suscriptor->socket = socket;
	suscriptor->mensajes_recibidos = list_create();
	return suscriptor;
}

int suscriptor_ya_recibio_mensaje(t_list* mensajes_recibidos, int id_mensaje){
	int recibio_mensaje = 0;

	for (int i = 0; i < list_size(mensajes_recibidos); i++) {
		int* id = list_get(mensajes_recibidos, i);
		if ((*id) == id_mensaje) {
			recibio_mensaje = 1;
			i = list_size(mensajes_recibidos);
		}
	}

	return recibio_mensaje;
}

void suscriptor_agregar_mensaje_recibido(t_list* mensajes_recibidos, int id_mensaje){
	int* id = malloc(sizeof(int));
	*id = id_mensaje;
	list_add(mensajes_recibidos,id);
}

int suscriptor_esta_suscripto(t_list* lista_suscriptores, int pid){
	int pos = -1;
	for (int i = 0; i < list_size(lista_suscriptores); i++){
		t_suscriptor* nuevo = list_get(lista_suscriptores,i);
		if (nuevo->pid == pid){
			pos = i;
		}
	}
	return pos;
}

char* suscriptor_to_string(t_suscriptor* suscriptor){
	int socket = suscriptor->socket;
	int32_t id = suscriptor->pid;
	//TODO Conditional jump or move depends on uninitialised value(s)
	return string_from_format("ID suscriptor: %d conectado a traves del socket %d ",id,socket);
}

void suscriptor_suscribirse_a(t_list* lista_suscriptores, t_suscriptor* suscriptor){
	list_add(lista_suscriptores,suscriptor);
}

void suscriptor_reconectar(t_list* lista_suscriptores, int* socket, int posicion){
	t_suscriptor* suscriptor = list_get(lista_suscriptores,posicion);
	suscriptor->socket = *socket;
}

void suscriptor_destroy(void* suscriptor){
	list_destroy_and_destroy_elements(((t_suscriptor*)suscriptor)->mensajes_recibidos,free);
	free((t_suscriptor*)suscriptor);
}
