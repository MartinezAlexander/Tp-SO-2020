#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdlib.h>
#include <commons/log.h>
t_log* logger;

void iniciar_logger(char* path);
void loggear_info(char* info);
void logger_destroy();

#endif
