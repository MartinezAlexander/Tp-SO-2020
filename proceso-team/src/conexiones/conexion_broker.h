/*
 * conexion_broker.h
 *
 *  Created on: 16 may. 2020
 *      Author: utnso
 */

#ifndef CONEXION_BROKER_H_
#define CONEXION_BROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<commons/string.h>
#include<commons/collections/list.h>
#include<mensajes/mensajes.h>
#include<mensajes/suscripcion.h>
#include<mensajes/client.h>
#include<mensajes/localized_pokemon.h>
#include<mensajes/appeared_pokemon.h>
#include<mensajes/caught_pokemon.h>
#include<mensajes/get_pokemon.h>


#include "../mensajes/procesamiento_caught.h"
#include "../mensajes/procesamiento_appeared.h"
#include "../mensajes/procesamiento_localized.h"
#include "../utils/log_utils.h"


void iniciar_conexion_broker();
void iniciar_suscripcion_broker(op_code cola);
void recibir_mensaje_appeared(int socket_appeared);
void recibir_mensaje_caught(int socket_caught);
void recibir_mensaje_localized(int socket_localized);

void enviar_get_objetivo(t_list* objetivo_global);

#endif /* CONEXION_BROKER_H_ */
