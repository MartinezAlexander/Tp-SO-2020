#ifndef SUSCRIPTOR_H_
#define SUSCRIPTOR_H_

#include <commons/collections/list.h>
#include <stdlib.h>

typedef struct{
	int socket;
	int32_t pid;
	t_list* mensajes_recibidos;
}t_suscriptor;

t_suscriptor* suscriptor_create(int socket, int32_t pid);
int suscriptor_ya_recibio_mensaje(t_list* mensajes_recibidos, int id_mensaje);
void suscriptor_agregar_mensaje_recibido(t_list* mensajes_recibidos, int id_mensaje);
void suscriptor_destroy(void*);
int suscriptor_esta_suscripto(t_list* lista_suscriptores, int pid);
void suscriptor_suscribirse_a(t_list* lista_suscriptores, t_suscriptor* suscriptor);
void suscriptor_reconectar(t_list* lista_suscriptores, int* socket, int posicion);
char* suscriptor_to_string(t_suscriptor* suscriptor);

#endif /* SUSCRIPTOR_H_ */
