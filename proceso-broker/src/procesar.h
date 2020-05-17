
#ifndef PROCESAR_H_
#define PROCESAR_H_

#include <mensajes/mensajes.h>
#include <mensajes/suscripcion.h>
#include "cola.h"

void procesar_suscripcion(t_mensaje* mensaje, int* socket);
void procesar_pokemon(t_cola_mensajeria* cola);

#endif /* PROCESAR_H_ */
