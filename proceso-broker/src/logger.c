#include "logger.h"

void iniciar_logger(char* path){
	if((logger = log_create(path,"proceso-broker.c",true,LOG_LEVEL_INFO)) == NULL)
	{
		printf("No se pudo crear el log\n");
		exit(1);
	}
}

void loggear_info(char* info){
	log_info(logger,info);
}

void logger_destroy(){
	log_destroy(logger);
}
