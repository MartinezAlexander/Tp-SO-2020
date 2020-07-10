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

		char* especie_path = path(dir_files, especie);
		free(dir_files);
		char* pokemon_path = path(especie_path, "Metadata.bin");
		free(especie_path);

		return pokemon_path;
	} else {
		free(dir_files);
		char* pokemon_file_path = pokemon_file_create(especie);
		char* pokemon_path = path(pokemon_file_path, "Metadata.bin");
		free(pokemon_file_path);
		return pokemon_path;

	}
}

void agregar_pokemon(char* archivo, t_posicion posicion, int cantidad) {

	t_config* config_metadata = config_create(archivo);
	if (config_metadata == NULL)
		printf("Error al crear config de metadata\n");

	char** bloques_array = config_get_array_value(config_metadata, "BLOCKS");
	//2 bytes in 1 blocks are definitely lost

	int bloque = obtener_bloque_con_posicion(bloques_array, posicion, cantidad);

	if (bloque > -1) {
		//Algun bloque ya tiene la posicion y tiene lugar para sumarle la cantidad
		agregar_pokemon_a_bloque(bloque, posicion, cantidad);
	} else {
		//No hay bloques que tengan la posicion con lugar para sumarle
		bloque = obtener_primer_bloque_con_espacio(bloques_array, posicion,
				cantidad);

		if (bloque < 0) {
			bloque = obtener_bloque_disponible();

			//TODO: (luego de responder duda) catchear caso extremo de que devuelva -1
			// (no hay bloques libres)

			crear_block(bloque);

			//Actualizo los bloques en el metadata
			char* bloques_string = config_get_string_value(config_metadata,
					"BLOCKS");
			bloques_string = string_substring(bloques_string, 0,
					string_length(bloques_string) - 1);
			char* bloques_actualizados = string_from_format("%s,%d]",
					bloques_string, bloque);

			config_set_value(config_metadata, "BLOCKS", bloques_actualizados);
			free(bloques_actualizados);
			free(bloques_string);
		}

		agregar_nuevo_pokemon_a_bloque(bloque, posicion, cantidad);
	}

	char* size = string_itoa(obtener_tamanio_listado_de_bloques(bloques_array));
	config_set_value(config_metadata, "SIZE", size);
	free(size);

	config_save(config_metadata);
	config_destroy(config_metadata);

	free(bloques_array);
}

/*
 * Decrementa la cantidad de pokemones en la posicion dada para tal archivo pokemon.
 * En caso de que no queden mas luego de decrementar, elimina la posicion del archivo.
 */
void decrementar_cantidad(char* archivo, t_posicion posicion) {
	t_config* config_metadata = config_create(archivo);
	if (config_metadata == NULL)
		printf("Error al crear config de metadata\n");

	char** bloques_array = config_get_array_value(config_metadata, "BLOCKS");

	//2 bytes in 1 blocks are definitely lost

	int bloque = obtener_bloque_con_posicion(bloques_array, posicion, 0);

	int elimino_fila = eliminar_pokemon_de_bloque(bloque, posicion);
	if (elimino_fila) {
		int tamanio_bloque = obtener_tamanio_ocupado_por_bloque(bloque);

		if (tamanio_bloque == 0) {
			liberar_bloque(bloque);
			char* bloques_actualizados = obtener_string_bloques_sin(
					bloques_array, bloque);
			config_set_value(config_metadata, "BLOCKS", bloques_actualizados);
			free(bloques_actualizados);
		}
		//TODO: (luego de responder duda) Si el bloque queda vacio se elimina??
		//En ese caso habria que chequear si pasa esto
		//Eliminarlo de bitmap y eliminar archivo
		//Sacarlo del array de bloques dentro del metadata

		//En caso de que no se borre, habria que actualizar el bitmap
	}

	char* size = string_itoa(obtener_tamanio_listado_de_bloques(bloques_array));
	config_set_value(config_metadata, "SIZE", size);
	free(size);

	config_save(config_metadata);
	config_destroy(config_metadata);
	free(bloques_array);
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

	//2 bytes in 1 blocks are definitely lost

	int index = 0;
	while (bloques_array[index] != NULL) {
		int bloque = atoi(bloques_array[index]);

		t_list* posiciones_bloque = obtener_posiciones_de_bloque(bloque);
		list_add_all(posiciones, posiciones_bloque);
		list_destroy(posiciones_bloque);

		index++;
	}

	free(bloques_array);
	config_destroy(config_metadata);

	return posiciones;
}

int existe_posicion(char* archivo, t_posicion posicion) {

	t_config* config_metadata = config_create(archivo);
	if (config_metadata == NULL)
		printf("Error al crear config de metadata\n");

	char** bloques_array = config_get_array_value(config_metadata, "BLOCKS");

//2 bytes in 1 blocks are definitely lost

	config_destroy(config_metadata);

	int bloque = obtener_bloque_con_posicion(bloques_array, posicion, 0);

	free(bloques_array);

	return bloque > -1;
}

void abrir_archivo(char* archivo) {

//Uso config para leer el archivo metadata
	t_config* config_metadata = config_create(archivo);
	if (config_metadata == NULL)
		printf("Error al crear config de metadata\n");

	pthread_mutex_lock(&mutex_modificacion_de_archivo);
	char* valor_open = config_get_string_value(config_metadata, "OPEN");
	pthread_mutex_unlock(&mutex_modificacion_de_archivo);

//En caso que ya este abiero espero x segundos
	while (string_equals_ignore_case(valor_open, "Y")) {
		sleep(tiempo_reintento_operacion);
		printf("-Reintento abrir archivo- ");

		t_config* config_metadata_actualizado = config_create(archivo);

		pthread_mutex_lock(&mutex_modificacion_de_archivo);
		valor_open = config_get_string_value(config_metadata_actualizado,
				"OPEN");
		pthread_mutex_unlock(&mutex_modificacion_de_archivo);

		config_destroy(config_metadata_actualizado);
	}

	pthread_mutex_lock(&mutex_modificacion_de_archivo);
	config_set_value(config_metadata, "OPEN", "Y");
	config_save(config_metadata);
	pthread_mutex_unlock(&mutex_modificacion_de_archivo);

	config_destroy(config_metadata);

	printf("-Se abrio archivo-\n");
}

void cerrar_archivo(char* archivo) {
//Uso config para leer el archivo metadata
	t_config* config_metadata = config_create(archivo);
	if (config_metadata == NULL)
		printf("Error al crear config de metadata\n");

	pthread_mutex_lock(&mutex_modificacion_de_archivo);
	config_set_value(config_metadata, "OPEN", "N");
	config_save(config_metadata);
	pthread_mutex_unlock(&mutex_modificacion_de_archivo);

	config_destroy(config_metadata);

	printf("-Se cerro archivo-\n");
}

char* pokemon_file_create(char* especie) {

	char* directorio_files = obtener_directorio_files();
	char* path_pokemon = path(directorio_files, especie);
	crear_directorio(directorio_files, especie);

	free(directorio_files);

	crear_metadata(path_pokemon);

	return path_pokemon;
}

void crear_metadata(char* ruta) {
	int bloque_numero = obtener_bloque_disponible();
	crear_block(bloque_numero);

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
