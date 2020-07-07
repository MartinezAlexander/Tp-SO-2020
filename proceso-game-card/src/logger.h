/*
 * logger.h
 *
 *  Created on: 4 jul. 2020
 *      Author: utnso
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <mensajes/mensajes.h>
#include <mensajes/suscripcion.h>

t_log* logger;

void iniciar_logger(char* path);
void loggear_info(char* info);

void loggear_conexion(int socket);
char* proceso_to_string(t_proceso id_proceso);
#endif /* LOGGER_H_ */
