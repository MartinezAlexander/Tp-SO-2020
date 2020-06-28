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
	//iniciar_conexion_broker();
	//iniciar_puerto_escucha();

	terminar_programa();
}

void inicializar_variables_globales(){
	config = leer_config();

	tiempo_reintento_conexion = config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION");
	tiempo_reintento_operacion = config_get_int_value(config, "TIEMPO_DE_REINTENTO_OPERACION");
	tiempo_retardo_operacion = config_get_int_value(config, "TIEMPO_RETARDO_OPERACION");
	punto_de_montaje_tallgrass = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
	ip_broker = config_get_string_value(config, "IP_BROKER");
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");
	iniciar_logger(config_get_string_value(config, "LOG_FILE"));
	//TODO inicializar correctamente (es el sustituto del process id, como haciamos en team)
	card_id = 1;
}

void inicializar_filesystem(){
	//Primero levantamos las variables de Metadata/Metadata.bin
	char* metadata_dir = path(punto_de_montaje_tallgrass, "Metadata");
	char* metadata_path = path(metadata_dir, "Metadata.bin");

	FILE* metadata = fopen(metadata_path, "r");

	t_config* config = config_create(metadata_path);
	if(config == NULL) puts("Error al leer Metadata/Metadata.bin");

	block_size = config_get_int_value(config, "BLOCK_SIZE");
	blocks = config_get_int_value(config, "BLOCKS");
	config_destroy(config);

	fclose(metadata);

	printf("[Inicializacion] Block_Size leido: %d\n", block_size);
	printf("[Inicializacion] Blocks leidos: %d\n", blocks);

	//Lo mismo con el Bitmap, es ir al path, leer el archivo y listo
	char* bitmap_path = path(metadata_dir, "Bitmap.bin");
	FILE* bitmap = fopen(bitmap_path, "r");

	int bitarray_size = blocks / 8;
	void* puntero_a_bits = malloc(bitarray_size);
	t_bitarray* bitarray = bitarray_create(puntero_a_bits,bitarray_size);

	int index = 0;
	char buffer;


	fread(&buffer, 1, 1, bitmap);
	while(buffer != EOF){
		if(buffer == '1'){
			bitarray_set_bit(bitarray, index);
		}else{
			bitarray_clean_bit(bitarray, index);
		}
		index++;
		fread(&buffer, 1, 1, bitmap);
		printf("Index: %d \n", index);
	}

	//Caso raro: En caso de que no nos den bitmap, tendriamos que armarlo nosotros
	//Depues de levantar los poke
	//Vamos por cada metadata, vemos que bloques tiene, nos armamos los bloques
	//y ahi ya podemos armar el bitmap

	//Caso raro: no nos dan bloques => no importa
	//Los bloques una vez que sabemos el numero de bloques y tenemos el bitmap
	// vamos a Blocks/x.bin, segun esten en el bitmap

	//Levantamos pokemones:
	//Entramos a /Files, de alguna manera deberiamos obtener que directorios tiene
	//adentro.
	//Por cada directorio entro a /Files/Dir/Metadata.bin y levanto
}

t_log* iniciar_logger(char* path){
	t_log* logger;
	if((logger = log_create(path,"gamecard",true,LOG_LEVEL_INFO)) == NULL)
	{
		printf("No se pudo crear el log\n");
		exit(1);
	}
	return logger;
}

t_config* leer_config(void){
	t_config *config;
	if((config = config_create("../src/gamecard.config")) == NULL)
	{
		printf("No pude leer la config\n");
		exit(2);
	}
	return config;
}

void terminar_programa(){

	if(logger != NULL)
	{
		log_info(logger, "finalizando programa...");
		log_destroy(logger);
	}
	if(config != NULL)
	{
		config_destroy(config);
	}
}

