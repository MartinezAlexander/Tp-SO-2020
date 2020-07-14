#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>

t_log* logger;

t_log* logger_personal;

void iniciar_logger(char* path);
void loggear_info(char* info);
void loggear_recepcion_ACK(char* suscriptor);
void loggear_envio_mensaje(char* mensaje);
void loggear_recepcion_mensaje(char* mensaje);
void loggear_suscripcion_proceso(char* suscriptor);
void loggear_reconexion_proceso(char* suscriptor);
void loggear_conexion_al_broker(int socket_cliente);
void loggear_mensaje_cacheado(char* mensaje, int base_particion);
void logger_destroy();

void iniciar_log_personal(char* path);
void log_personal_error_envio_a_suscriptor(char* suscriptor);
void log_personal_destroy();
void log_personal_error_cacheo_mensaje(int32_t id);

#endif
