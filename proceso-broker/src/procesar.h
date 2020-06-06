#ifndef PROCESAR_H_
#define PROCESAR_H_

#include <mensajes/mensajes.h>
#include <mensajes/suscripcion.h>
#include <commons/string.h>
#include "cola.h"
#include "memoria_cache.h"
#include <pthread.h>
#include "logger.h"
#include "particiones_dinamicas/administrador_particiones.h"

typedef struct{
	int32_t id_mensaje;
	t_list* enviados;
	t_list* fallidos;
}t_estado_mensaje;

t_estado_mensaje* estado_mensaje_create(int32_t id);
void procesar_suscripcion(t_mensaje* mensaje, int* socket);
void procesar_pokemon(t_cola_mensajeria* cola);
void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje);

#endif
