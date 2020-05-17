#ifndef SUSCRIPTOR_H_
#define SUSCRIPTOR_H_

#include <commons/collections/list.h>
#include <stdlib.h>

typedef struct{
	int socket;
	int32_t pid;
}t_suscriptor;

t_suscriptor* suscriptor_create(int socket, int32_t pid);
void suscriptor_destroy(void*);
int suscriptor_esta_suscripto(t_list* lista_suscriptores, t_suscriptor* suscriptor);
void suscriptor_suscribirse_a(t_list* lista_suscriptores, t_suscriptor* suscriptor);
void suscriptor_reconectar(t_list* lista_suscriptores, t_suscriptor* suscriptor, int posicion);

#endif /* SUSCRIPTOR_H_ */
