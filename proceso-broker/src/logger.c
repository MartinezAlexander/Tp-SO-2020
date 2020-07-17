#include "logger.h"

void iniciar_logger(char* path){
	if((logger = log_create(path,"proceso-broker.c",false,LOG_LEVEL_INFO)) == NULL)
	{
		printf("No se pudo crear el log\n");
		exit(1);
	}
}

void loggear_info(char* info){
	log_info(logger,info);
}

void loggear_recepcion_ACK(char* suscriptor){
	char* log = string_from_format("[ACK] Recibi confirmacion de %s\n-----------------------------------------------------------------",suscriptor);
	log_info(logger,log);
	free(log);
	free(suscriptor);
}

void loggear_envio_mensaje(char* mensaje){
	char* log = string_from_format("[Enviado] %s\n-----------------------------------------------------------------",mensaje);
	log_info(logger,log);
	free(log);
	free(mensaje);
}

void loggear_recepcion_mensaje(char* mensaje){
	char* log = string_from_format("[Recibido] %s\n-----------------------------------------------------------------", mensaje);
	log_info(logger,log);
	free(log);
	free(mensaje);
}

void loggear_suscripcion_proceso(char* suscriptor){
	char* log = string_from_format("[Suscripcion] %s\n-----------------------------------------------------------------",suscriptor);
	log_info(logger,log);
	free(log);
	free(suscriptor);
}

void loggear_reconexion_proceso(char* suscriptor){
	char* log = string_from_format("[Reconexion] %s\n-----------------------------------------------------------------",suscriptor);
	log_info(logger,log);
	free(log);
	free(suscriptor);
}

void loggear_conexion_al_broker(int socket_cliente){
	char* log = string_from_format("[Conexion] Proceso se conecto a traves del socket %d\n-----------------------------------------------------------------", socket_cliente);
	log_info(logger,log);
	free(log);
}

void loggear_mensaje_cacheado(char* mensaje, int base_particion){
	char* log = string_from_format("[Cache] Mensaje: %s | almacenado en la particion con base %d\n-----------------------------------------------------------------",mensaje,base_particion);
	log_info(logger,log);
	free(log);
	free(mensaje);
}

void loggear_compactacion(int tamanio_libre, int base_libre){
	log_info(logger,"[Compactacion] Ejecutado: Tamanio de particion libre final %d - Base de particion libre final %d\n-----------------------------------------------------------------",tamanio_libre,base_libre);
}

void loggear_eliminacion_particion(int base_eliminada){
	log_info(logger,"[Eliminacion] Ejecutado: Elimino particion con base: %d \n-----------------------------------------------------------------",base_eliminada);
}

void loggear_asociacion_bloques(int base_bloque1, int base_bloque2){
	log_info(logger,"[Asociacion de bloques] Ejecutado: Base particiones asociadas %d %d\n-----------------------------------------------------------------",base_bloque1,base_bloque2);
}

void loggear_ejecucion_dump(){
	log_info(logger,"[Dump] Se solicito la ejecucion del dump de la cache: solicitud procesada\n-----------------------------------------------------------------");
}

void logger_destroy(){
	if(logger != NULL){
		log_info(logger,"/////////////////////////////////////////////////////////////////////////////////////////////////////////////////");
		log_destroy(logger);
	}
}

void iniciar_log_personal(char* path){
	if((logger_personal = log_create(path,"proceso-broker.c",false,LOG_LEVEL_INFO)) == NULL)
	{
		printf("No se pudo crear el log personal\n");
		exit(1);
	}
}

void log_personal_error_envio_a_suscriptor(char* suscriptor){
	char* string = string_from_format("[Envio fallido] a %s\n-----------------------------------------------------------------",suscriptor);
	log_error(logger_personal,string);
	free(string);
	free(suscriptor);
}

void log_personal_error_cacheo_mensaje(int32_t id){
	char* string = string_from_format("[Cacheo fallido] El mensaje con id %d no entra en la cache\n----------------------------",id);
	log_error(logger_personal,string);
	free(string);
}


void log_personal_destroy(){
	if(logger_personal!=NULL){
		log_destroy(logger_personal);
	}
}
