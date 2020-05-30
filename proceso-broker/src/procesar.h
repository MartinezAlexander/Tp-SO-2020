
#ifndef PROCESAR_H_
#define PROCESAR_H_

#include <mensajes/mensajes.h>
#include <mensajes/suscripcion.h>
#include "cola.h"
#include "memoria_cache.h"
#include <pthread.h>

//TODO como llamar al logger en el interior de estas funciones
#include<commons/log.h>
t_log* loger;
void obtener_logger(t_log* logger);

//TODO resolucion turbia para evitar hacer una struct en el hilo de procesar pokemon
t_memoria_cache* memoria;
void obtener_cache(t_memoria_cache* memoria);

typedef struct{
	int32_t id_mensaje;
	t_list* enviados;
	t_list* fallidos;
}t_estado_mensaje;

t_estado_mensaje* estado_mensaje_create(int32_t id);
void procesar_suscripcion(t_mensaje* mensaje, int* socket,t_memoria_cache* memoria);
void procesar_pokemon(t_cola_mensajeria* cola);
void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje);

#endif /* PROCESAR_H_ */
