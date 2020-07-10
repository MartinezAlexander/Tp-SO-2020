/*
 ============================================================================
 Name        : proceso-game-card.c
 Author      : Grupo 7 - SO
 Version     : v1.0
 Copyright   : Your copyright notice
 Description : Almacena los datos los datos de los distintos Pokemon que se
 encuentren en el mapa.
 ============================================================================
 */

#include "proceso-game-card.h"

int main(void) {
	inicializar_variables_globales();

	inicializar_filesystem();

	//test_agregar_cantidad_a_archivo();

	//iniciar_conexion_broker();

	iniciar_puerto_de_escucha();

	terminar_programa();
}

void inicializar_variables_globales() {
	config = leer_config();

	tiempo_reintento_conexion = config_get_int_value(config,
			"TIEMPO_DE_REINTENTO_CONEXION");
	tiempo_reintento_operacion = config_get_int_value(config,
			"TIEMPO_DE_REINTENTO_OPERACION");
	tiempo_retardo_operacion = config_get_int_value(config,
			"TIEMPO_RETARDO_OPERACION");
	punto_de_montaje_tallgrass = config_get_string_value(config,
			"PUNTO_MONTAJE_TALLGRASS");
	ip_broker = config_get_string_value(config, "IP_BROKER");
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");
	iniciar_logger(config_get_string_value(config, "LOG_FILE"));
	card_id = config_get_int_value(config, "ID_GAMECARD");
	ip_gamecard = config_get_string_value(config, "IP_GAMECARD");
	puerto_gamecard = config_get_string_value(config, "PUERTO_GAMECARD");
}

void inicializar_filesystem() {

	//TODO PREGUNTAR el metadata si o si debe existir, sino deberiamos inventar blocks y blocks_size

	metadata_cargar();

	bitmap_cargar();

	pthread_mutex_init(&mutex_modificacion_de_archivo, NULL);

	/*
	 * Decidimos levantar solamente el metadata y el bitmap. Los bloques y archivos
	 * pokemon los traemos cuando los necesitamos
	 */
}

t_config* leer_config(void) {
	t_config *config;
	if ((config = config_create("../src/gamecard.config")) == NULL) {
		printf("No pude leer la config\n");
		exit(2);
	}
	return config;
}

void terminar_programa() {

	if (logger != NULL) {
		log_info(logger, "finalizando programa...");
		log_destroy(logger);
	}
	if (config != NULL) {
		config_destroy(config);
	}
}

