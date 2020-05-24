/*
 * conexion_gameboy.h
 *
 *  Created on: 16 may. 2020
 *      Author: utnso
 */

#ifndef CONEXION_GAMEBOY_H_
#define CONEXION_GAMEBOY_H_

#include<stdio.h>
#include<stdlib.h>
#include<mensajes/server.h>
#include<mensajes/mensajes.h>
#include<mensajes/localized_pokemon.h>
#include<mensajes/appeared_pokemon.h>
#include<mensajes/caught_pokemon.h>
#include "../mensajes/procesamiento_caught.h"
#include "../mensajes/procesamiento_appeared.h"
#include "../mensajes/procesamiento_localized.h"
#include "../variables_globales.h"

void iniciar_puerto_de_escucha();
void procesar_mensajes_directos(int* socket);


#endif /* CONEXION_GAMEBOY_H_ */
