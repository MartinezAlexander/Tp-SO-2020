/*
 * file_pokemon.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "file_pokemon.h"

char* obtener_pokemon(char* especie) {
	char* dir_files = obtener_directorio_files();
	if (existe_archivo_en(especie, dir_files)) {
		printf("La especie %s existe\n",especie);
		char* especie_path = path(dir_files, especie);
		free(dir_files);
		char* pokemon_path = path(especie_path, "Metadata.bin");
		free(especie_path);

		return pokemon_path;
	} else {
		printf("La especie %s no existe\n",especie);
		free(dir_files);
		char* pokemon_file_path = pokemon_file_create(especie);
		char* pokemon_path = path(pokemon_file_path, "Metadata.bin");
		free(pokemon_file_path);
		return pokemon_path;

	}
}

void agregar_pokemon(char* archivo, t_posicion posicion, int cantidad) {

	puts("[Agregar pokemon]: Abro config");
	t_config* config_metadata = config_create(archivo);
	if (config_metadata == NULL)
		printf("Error al crear config de metadata\n");

	puts("Leo los bloques");
	char** bloques_array = config_get_array_value(config_metadata, "BLOCKS");
	//2 bytes in 1 blocks are definitely lost

	puts("Obteniendo renglones");
	t_list* renglones = obtener_posiciones_de_bloques(bloques_array);

	puts("Agregar info posicion a lista");
	agregar_info_posicion_a_listado(renglones, posicion, cantidad);

	puts("Calcular nuevo tamaño de posiciones");
	int tamanio = tamanio_info_posiciones(renglones);

	puts("Calculo posible bloque nuevo");
	int cantidad_bloques_actuales = array_cantidad_de_elementos(bloques_array);
	int capacidad_bloques_actuales = block_size * cantidad_bloques_actuales;
	free(bloques_array);

	if(tamanio > capacidad_bloques_actuales){
		puts("Busco un bloque nuevo");
		int nuevo_bloque = obtener_bloque_disponible();
		crear_block(nuevo_bloque);

		puts("Agrego bloque nuevo");
		char* bloques_string = config_get_string_value(config_metadata,"BLOCKS");
		bloques_string = string_substring(bloques_string, 0, string_length(bloques_string) - 1);
		char* bloques_actualizados = string_from_format("%s,%d]", bloques_string, nuevo_bloque);
		config_set_value(config_metadata, "BLOCKS", bloques_actualizados);
		config_save(config_metadata);
		free(bloques_string);
	}

	puts("Leo nueva lista de bloques asignados");
	bloques_array = config_get_array_value(config_metadata, "BLOCKS");

	puts("Actualizo los bloques");
	actualizar_bloques(bloques_array, renglones);
	free(bloques_array);
	list_destroy_and_destroy_elements(renglones,free);

	puts("Actualizo el size");
	char* size = string_itoa(tamanio);
	config_set_value(config_metadata, "SIZE", size);
	config_save(config_metadata);
	free(size);
	config_destroy(config_metadata);
}

/*
 * Decrementa la cantidad de pokemones en la posicion dada para tal archivo pokemon.
 * En caso de que no queden mas luego de decrementar, elimina la posicion del archivo.
 */
int decrementar_cantidad(char* archivo, t_posicion posicion) {
	t_config* config_metadata = config_create(archivo);
	if (config_metadata == NULL)
		printf("Error al crear config de metadata\n");

	char** bloques_array = config_get_array_value(config_metadata, "BLOCKS");
	//2 bytes in 1 blocks are definitely lost



	t_list* renglones = obtener_posiciones_de_bloques(bloques_array);
	int existe_posicion = decrementar_info_posicion_en_listado(renglones, posicion);

	if(!existe_posicion){
		free(bloques_array);
		list_destroy(renglones);
		config_destroy(config_metadata);
		return 0;
	}

	int tamanio = tamanio_info_posiciones(renglones);

	int bloques_liberados = actualizar_bloques(bloques_array, renglones);
	list_destroy(renglones);
	free(bloques_array);

	char* bloques_string = config_get_string_value(config_metadata,"BLOCKS");
	bloques_string = string_substring(bloques_string, 0, string_length(bloques_string) - 1);
	bloques_string = string_substring(bloques_string, 0, string_length(bloques_string) - bloques_liberados * 2);
	bloques_string = string_from_format("%s]", bloques_string);

	if(!string_starts_with(bloques_string,"[")){
		bloques_string = string_from_format("[%s", bloques_string);
	}

	config_set_value(config_metadata,"BLOCKS", bloques_string);
	free(bloques_string);

	char* size = string_itoa(tamanio);
	config_set_value(config_metadata, "SIZE", size);
	free(size);

	config_save(config_metadata);
	config_destroy(config_metadata);

	return 1;
}

/*
 * Dado un archivo pokemon, devuelve una lista con todas
 * las posiciones en las que se encuentra dicho pokemon
 */
t_list* obtener_posiciones_pokemon(char* archivo) {
	t_list* posiciones = list_create();

	t_config* config_metadata = config_create(archivo);
	if (config_metadata == NULL)
		printf("Error al crear config de metadata\n");

	char** bloques_array = config_get_array_value(config_metadata, "BLOCKS");
	config_destroy(config_metadata);
	//2 bytes in 1 blocks are definitely lost
	t_list* renglones = obtener_posiciones_de_bloques(bloques_array);
	free(bloques_array);

	for(int i = 0 ; i < list_size(renglones) ; i++){
		info_posicion* renglon = list_get(renglones, i);
		t_posicion* posicion = malloc(sizeof(t_posicion));
		posicion->posicionX = renglon->posicion.posicionX;
		posicion->posicionY = renglon->posicion.posicionY;
		list_add(posiciones, posicion);
	}

	return posiciones;
}

int existe_posicion(char* archivo, t_posicion posicion) {

	t_config* config_metadata = config_create(archivo);
	if (config_metadata == NULL)
		printf("Error al crear config de metadata\n");

	char** bloques_array = config_get_array_value(config_metadata, "BLOCKS");
	config_destroy(config_metadata);

	int bloque = obtener_bloque_con_posicion(bloques_array, posicion, 0);



	free(bloques_array);

	return bloque > -1;
}

void abrir_archivo(char* archivo, int hilo){

//Uso config para leer el archivo metadata

	//pthread_mutex_lock(&mutex_modificacion_de_archivo);
	t_config* config_metadata = config_create(archivo);
	if (config_metadata == NULL)
		printf("Error al crear config de metadata\n");

	char* valor_open = config_get_string_value(config_metadata, "OPEN");
	printf("Soy el hilo %d y leo el open\n",hilo);
	//pthread_mutex_unlock(&mutex_modificacion_de_archivo);

//En caso que ya este abiero espero x segundos
	while (string_equals_ignore_case(valor_open, "Y")) {

		sleep(tiempo_reintento_operacion);
		printf("Soy el hilo %d y reintento abrir archivo- \n",hilo);

		//pthread_mutex_lock(&mutex_modificacion_de_archivo);
		t_config* config_metadata_actualizado = config_create(archivo);
		valor_open = config_get_string_value(config_metadata_actualizado, "OPEN");
		printf("Soy el hilo %d y leo el open (reintentando)\n",hilo);
		//pthread_mutex_unlock(&mutex_modificacion_de_archivo);

		//config_destroy(config_metadata_actualizado);
	}

	//pthread_mutex_lock(&mutex_modificacion_de_archivo);
	config_set_value(config_metadata, "OPEN", "Y");
	config_save(config_metadata);
	printf("Soy el hilo %d y actualizo el open\n",hilo);
	//pthread_mutex_unlock(&mutex_modificacion_de_archivo);

	config_destroy(config_metadata);

	printf("-Soy el hilo %d Se abrio archivo-\n",hilo);
}

void cerrar_archivo(char* archivo) {
//Uso config para leer el archivo metadata
	//pthread_mutex_lock(&mutex_modificacion_de_archivo);
	t_config* config_metadata = config_create(archivo);
	if (config_metadata == NULL)
		printf("Error al crear config de metadata\n");

	config_set_value(config_metadata, "OPEN", "N");
	config_save(config_metadata);
	//pthread_mutex_unlock(&mutex_modificacion_de_archivo);

	config_destroy(config_metadata);

	printf("-Se cerro archivo-\n");
}

char* pokemon_file_create(char* especie) {

	char* directorio_files = obtener_directorio_files();
	char* path_pokemon = path(directorio_files, especie);

	printf("Creo la carpeta de la especie %s\n",especie);

	//TODO usar el mismo mutex que el listar_directorios?
	crear_directorio(directorio_files, especie);

	free(directorio_files);

	crear_metadata(path_pokemon);

	return path_pokemon;
}

void crear_metadata(char* ruta) {
	int bloque_numero = obtener_bloque_disponible();
	crear_block(bloque_numero);

	puts("Creo el metadata");

	FILE* metadata = crear_archivo(ruta, "Metadata.bin");
	txt_write_in_file(metadata, "DIRECTORY=N\n");
	txt_write_in_file(metadata, "SIZE=0\n");
	char* bloque_string = string_itoa(bloque_numero);
	char* blocks = string_from_format("BLOCKS=[%s]\n", bloque_string);
	txt_write_in_file(metadata, blocks);
	free(bloque_string);
	free(blocks);
	txt_write_in_file(metadata, "OPEN=N\n");
	txt_close_file(metadata);
}
