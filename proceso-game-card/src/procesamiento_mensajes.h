/*
 * procesamiento_mensajes.h
 *
 *  Created on: 6 jun. 2020
 *      Author: utnso
 */

#ifndef PROCESAMIENTO_MENSAJES_H_
#define PROCESAMIENTO_MENSAJES_H_

#include <stdio.h>
#include <stdlib.h>
#include <mensajes/client.h>
#include <mensajes/mensajes.h>
#include <mensajes/new_pokemon.h>
#include <mensajes/catch_pokemon.h>
#include <mensajes/get_pokemon.h>
#include <mensajes/appeared_pokemon.h>
#include <mensajes/caught_pokemon.h>
#include <mensajes/localized_pokemon.h>
#include "logger.h"
#include "file_pokemon.h"

#include "tall_grass.h"
#include "variables_globales.h"

pthread_mutex_t mutex_obtener_pokemon;

/*
 * Dado un mensaje, crea un hilo que ejecuta a la funcion pasada por parametro
 * para atenderlo. (ejecutar_new, ejecutar_catch, o ejecutar_get)
 */
void procesar_mensaje(t_mensaje* mensaje_recibido, void* funcion_ejecutar);

void ejecutar_new(t_mensaje* mensaje_recibido);
void ejecutar_catch(t_mensaje* mensaje_recibido);
void ejecutar_get(t_mensaje* mensaje_recibido);

void enviar_mensaje_al_broker(t_mensaje* mensaje);

#endif /* PROCESAMIENTO_MENSAJES_H_ */
