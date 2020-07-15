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
#include <mensajes/posiciones.h>
#include <mensajes/mensajes.h>
#include <mensajes/suscripcion.h>

t_log* logger;

void iniciar_logger(char* path);
void loggear_info(char* info);

void loggear_error_conexion(int socket);
void loggear_no_existe_posicion(t_posicion posicion);
void loggear_no_existe_pokemon(char* especie);
void loggear_archivo_abierto(char* especie);
void loggear_reintento(char* especie);

char* proceso_to_string(t_proceso id_proceso);
#endif /* LOGGER_H_ */
