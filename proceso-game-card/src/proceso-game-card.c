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
	//iniciar_puerto_escucha();
/*
	FILE* fd = fopen("/home/utnso/Escritorio/tall-grass/Metadata/Bitmap.bin","a");
	char x = '0';
	if (fd != NULL) {
		for (int i = 0; i < 5192; i++) {
			fputc(x, fd);
		}
	}

	fclose(fd);
*/
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
	card_id = config_get_int_value(config,"ID_GAMECARD");
}

void inicializar_filesystem(){
	//Primero levantamos las variables de Metadata/Metadata.bin
	metadata_cargar();

	//Lo mismo con el Bitmap, es ir al path, leer el archivo y listo
	bitmap_cargar();

	/*puts("Bitmap antes de crear bloque 2");
	for(int i=0 ; i<5; i++){
		printf("Bloque %d - cero = libre - uno = ocupado - %c \n",i,estado_bloques->bitarray[i]);
	}*/

	 //Creo el bloque 2.bin
	/*t_config* bloque_dos = crear_block(2);
	// guardo 20 pokemones en la posicion 8,9
	config_set_value(bloque_dos,"8-9","20");
	config_set_value(bloque_dos,"1-2","3");
	config_set_value(bloque_dos,"4-4","1");
	config_save(bloque_dos);
	*/
	/*
	puts("Bitmap despues de crear bloque 2");
	for(int i=0 ; i<5; i++){
		printf("Bloque %d - cero = libre - uno = ocupado - %c \n",i,estado_bloques->bitarray[i]);
	}*/

	/*int tamanio = obtener_tamanio_ocupado_por_bloque(2);

	printf("Tamanio de bloque %d\n",tamanio);
	printf("Tamanio disponible: %d\n",bytes_libres_bloque(2));*/

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

