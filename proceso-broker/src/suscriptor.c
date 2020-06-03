#include "suscriptor.h"
#include <commons/string.h>

t_suscriptor* suscriptor_create(int socket, int32_t pid){
	t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
	suscriptor->pid = pid;
	suscriptor->socket = socket;
	return suscriptor;
}

int suscriptor_esta_suscripto(t_list* lista_suscriptores, t_suscriptor* suscriptor){
	int pos = -1;
	for (int i = 0; i < list_size(lista_suscriptores); i++){
		t_suscriptor* nuevo = list_get(lista_suscriptores,i);
		if (nuevo->pid == suscriptor->pid){
			pos = i;
		}
	}
	return pos;
}

char* suscriptor_to_string(t_suscriptor* suscriptor){
	return string_from_format("ID suscriptor: %d conectado a traves del socket %d ",suscriptor->pid,suscriptor->socket);
}

void suscriptor_suscribirse_a(t_list* lista_suscriptores, t_suscriptor* suscriptor){
	list_add(lista_suscriptores,suscriptor);
}

void suscriptor_reconectar(t_list* lista_suscriptores, t_suscriptor* suscriptor, int posicion){
	list_replace_and_destroy_element(lista_suscriptores, posicion, suscriptor, suscriptor_destroy);
}

void suscriptor_destroy(void* suscriptor){
	free((t_suscriptor*)suscriptor);
}
