#ifndef PROCESAR_H_
#define PROCESAR_H_

#include <mensajes/mensajes.h>
#include <mensajes/suscripcion.h>
#include <commons/string.h>
#include "cola.h"
#include "memoria_cache.h"
#include <pthread.h>

#include "logger.h"
#include "manejador_suscripciones.h"

void procesar_suscripcion();

void procesar_pokemon(t_cola_mensajeria* cola);

void envio_a_suscriptores(t_list* suscriptores, t_mensaje* mensaje);

#endif
