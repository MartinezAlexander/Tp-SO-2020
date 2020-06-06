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
#include <mensajes/mensajes.h>

void ejecutar_new(t_mensaje* mensaje);
void ejecutar_catch(t_mensaje* mensaje);
void ejecutar_get(t_mensaje* mensaje);

#endif /* PROCESAMIENTO_MENSAJES_H_ */
