
#include "suscriptor.h"

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

void suscriptor_suscribirse_a(t_list* lista_suscriptores, t_suscriptor* suscriptor){
	list_add(lista_suscriptores,suscriptor);
}

void suscriptor_reconectar(t_list* lista_suscriptores, t_suscriptor* suscriptor, int posicion){
	list_replace_and_destroy_element(lista_suscriptores, posicion, suscriptor, suscriptor_destroy);
}

void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje){
	for(int i = 0; i < list_size(suscriptores); i++){
		t_suscriptor* suscriptor = list_get(suscriptores,i);
		enviar_mensaje(mensaje,suscriptor->socket);
	}
}

void suscriptor_destroy(void* suscriptor){
	free((t_suscriptor*)suscriptor);
}