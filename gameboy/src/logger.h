/*
 * logger.h
 *
 *  Created on: 7 jun. 2020
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
void loggear_conexion(t_proceso id_proceso,int socket);
void loggear_suscripcion(op_code id_cola);
void loggear_nuevo_mensaje(op_code id_cola,char* mensaje);

char* proceso_to_string(t_proceso id_proceso);
#endif /* LOGGER_H_ */
