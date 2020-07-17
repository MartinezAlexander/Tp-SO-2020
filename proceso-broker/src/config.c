#include "config.h"

void iniciar_config(char* path_config){
	if((config = config_create(path_config)) == NULL)
	{
		printf("No pude leer la config\n");
		exit(2);
	}
}

void obtener_info_del_config(){
	tamano_memoria = config_get_int_value(config, "TAMANO_MEMORIA");
	tamano_minimo_particion = config_get_int_value(config,"TAMANO_MINIMO_PARTICION");

	algoritmo_particion_libre = config_get_string_value(config,"ALGORITMO_PARTICION_LIBRE");
	algoritmo_reemplazo = config_get_string_value(config,"ALGORITMO_REEMPLAZO");
	algoritmo_memoria = config_get_string_value(config, "ALGORITMO_MEMORIA");

	ip_broker = config_get_string_value(config, "IP_BROKER");
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");

	frecuencia_compactacion = config_get_int_value(config,"FRECUENCIA_COMPACTACION");

	path_logger = config_get_string_value(config, "LOG_FILE");
}

void finalizar_config(){
	if(config != NULL){
		config_destroy(config);
	}
}
