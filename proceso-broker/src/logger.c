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

void loggear_recepcion_ACK(char* suscriptor){
	char* log = string_from_format("[ACK] Recibi confirmacion de %s",suscriptor);
	log_info(logger,log);
	free(log);
	free(suscriptor);
}

void loggear_envio_mensaje(char* mensaje){
	char* log = string_from_format("[Enviado] %s",mensaje);
	log_info(logger,log);
	free(log);
	free(mensaje);
}

void loggear_recepcion_mensaje(char* mensaje){
	char* log = string_from_format("[Recibido] %s", mensaje);
	log_info(logger,log);
	free(log);
	free(mensaje);
}

void loggear_suscripcion_proceso(char* suscriptor){
	char* log = string_from_format("[Suscripcion] %s",suscriptor);
	log_info(logger,log);
	free(log);
	free(suscriptor);
}

void loggear_reconexion_proceso(char* suscriptor){
	char* log = string_from_format("[Reconexion] %s",suscriptor);
	log_info(logger,log);
	free(log);
	free(suscriptor);
}

void loggear_conexion_al_broker(int socket_cliente){
	char* log = string_from_format("[Conexion] Proceso se conecto a traves del socket %d", socket_cliente);
	log_info(logger,log);
	free(log);
}

void logger_destroy(){
	log_destroy(logger);
}

void iniciar_log_personal(char* path){
	if((logger_personal = log_create(path,"proceso-broker.c",true,LOG_LEVEL_INFO)) == NULL)
	{
		printf("No se pudo crear el log personal\n");
		exit(1);
	}
}

void log_personal_error_envio_a_suscriptor(char* suscriptor){
	char* string = string_from_format("[Envio fallido] a %s",suscriptor);
	log_error(logger_personal,string);
	free(string);
	free(suscriptor);
}

void log_personal_destroy(){
	log_destroy(logger_personal);
}
