/*
 * file_pokemon.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "file_pokemon.h"

file_pokemon* file_pokemon_obtener(char* especie){

	string_to_lower(especie);
	char c = especie[0];
	c = toupper(c);
	especie[0] = c;

	for(int i=0; i<list_size(pokemones_files); i++){
		file_pokemon* poke_file = list_get(pokemones_files,i);
		if(string_equals_ignore_case(especie,poke_file->especie)){
			return poke_file;
		}
	}

	file_pokemon* pokemon_file = malloc(sizeof(file_pokemon));
	pokemon_file->especie = string_new();
	string_append(&pokemon_file->especie,especie);
	pokemon_file->path = obtener_pokemon(especie);

	t_config* file = config_create(pokemon_file->path);
	char* _open = config_get_string_value(file, "OPEN");
	char* _blocks_string = config_get_string_value(file, "BLOCKS");
	char* _directory = config_get_string_value(file, "DIRECTORY");
	char* _size = config_get_string_value(file, "SIZE");

	pokemon_file->open = string_new();
	string_append(&pokemon_file->open,_open);
	pokemon_file->blocks_string = string_new();
	string_append(&pokemon_file->blocks_string,_blocks_string);
	pokemon_file->directory = string_new();
	string_append(&pokemon_file->directory,_directory);
	pokemon_file->size = string_new();
	string_append(&pokemon_file->size,_size);

	char* _blocks_string2 = string_substring(_blocks_string,1,strlen(_blocks_string)-2);
	pokemon_file->blocks = string_split(_blocks_string2,",");
	free(_blocks_string2);
	config_destroy(file);

	pthread_mutex_init(&pokemon_file->mutex_file,NULL);

	list_add(pokemones_files,pokemon_file);

	return pokemon_file;
}

void abrir_archivo_dos(file_pokemon* poke_file, int hilo){
	//Uso config para leer el archivo metadata

	pthread_mutex_lock(&poke_file->mutex_file);

	char* valor_open = poke_file->open;

	pthread_mutex_unlock(&poke_file->mutex_file);

	//En caso que ya este abiero espero x segundos
	while (string_equals_ignore_case(valor_open, "Y")) {

		loggear_archivo_abierto(poke_file->especie);
		sleep(tiempo_reintento_operacion);

		loggear_reintento(poke_file->especie);

		printf("Soy el hilo %d y reintento abrir archivo\n",hilo);

		pthread_mutex_lock(&poke_file->mutex_file);

		valor_open = poke_file->open;

		pthread_mutex_unlock(&poke_file->mutex_file);
	}

	pthread_mutex_lock(&poke_file->mutex_file);
	poke_file->open[0]='Y';
	pthread_mutex_unlock(&poke_file->mutex_file);

	printf("Soy el hilo %d y se abrio archivo de la especie",hilo);
}

void agregar_pokemon_dos(file_pokemon* poke_file, t_posicion posicion, int cantidad) {


	char** bloques_array = poke_file->blocks;

	t_list* renglones = obtener_posiciones_de_bloques(bloques_array);

	agregar_info_posicion_a_listado(renglones, posicion, cantidad);

	int tamanio = tamanio_info_posiciones(renglones);
	int cantidad_bloques_actuales = array_cantidad_de_elementos(bloques_array);
	int capacidad_bloques_actuales = block_size * cantidad_bloques_actuales;

	if(tamanio > capacidad_bloques_actuales){
		int nuevo_bloque = obtener_bloque_disponible();
		crear_block(nuevo_bloque);

		if(string_equals_ignore_case(poke_file->blocks_string,"[]")){
			char* bloques_string = poke_file->blocks_string;
			bloques_string = string_substring(bloques_string, 0, string_length(bloques_string) - 1);
			free(poke_file->blocks_string);
			poke_file->blocks_string = string_from_format("%s%d]", bloques_string, nuevo_bloque);
			free(bloques_string);
		}else{
			char* bloques_string = poke_file->blocks_string;
			bloques_string = string_substring(bloques_string, 0, string_length(bloques_string) - 1);
			free(poke_file->blocks_string);
			poke_file->blocks_string = string_from_format("%s,%d]", bloques_string, nuevo_bloque);
			free(bloques_string);
		}
		char* _blocks_string = string_substring(poke_file->blocks_string,1,strlen(poke_file->blocks_string)-2);
		free(poke_file->blocks);
		poke_file->blocks = string_split(_blocks_string,",");
		free(_blocks_string);
	}

	bloques_array = poke_file->blocks;
	actualizar_bloques(bloques_array, renglones);
	list_destroy_and_destroy_elements(renglones,free);
	free(poke_file->size);
	poke_file->size = string_itoa(tamanio);
}

void cerrar_archivo_dos(file_pokemon* poke_file) {

	pthread_mutex_lock(&poke_file->mutex_file);

	t_config* file = config_create(poke_file->path);
	config_set_value(file, "OPEN", "N");
	config_set_value(file, "BLOCKS", poke_file->blocks_string);
	config_set_value(file, "DIRECTORY", poke_file->directory);
	config_set_value(file, "SIZE", poke_file->size);
	config_save(file);
	config_destroy(file);


	poke_file->open[0] = 'N';

	pthread_mutex_unlock(&poke_file->mutex_file);

}


int decrementar_cantidad_dos(file_pokemon* poke_file, t_posicion posicion){

	char** bloques_array = poke_file->blocks;

	t_list* renglones = obtener_posiciones_de_bloques(bloques_array);
	int existe_posicion = decrementar_info_posicion_en_listado(renglones, posicion);

	if(!existe_posicion){
		loggear_no_existe_posicion(posicion);
		list_destroy_and_destroy_elements(renglones,free);
		return 0;
	}

	int tamanio = tamanio_info_posiciones(renglones);

	int bloques_liberados = actualizar_bloques(bloques_array, renglones);
	list_destroy_and_destroy_elements(renglones,free);

	char* bloques_aux0 = poke_file->blocks_string;
	char* bloques_aux1 = string_substring(bloques_aux0, 0, string_length(bloques_aux0) - 1);
	char* bloques_aux2 = string_substring(bloques_aux1, 0, string_length(bloques_aux1) - bloques_liberados * 2);
	char* bloques_string = string_from_format("%s]", bloques_aux2);
	free(bloques_aux0);
	free(bloques_aux1);
	free(bloques_aux2);
	if(!string_starts_with(bloques_string,"[")){
		bloques_string = string_from_format("[%s", bloques_string);
	}

	poke_file->blocks_string = bloques_string;

	if(string_equals_ignore_case(poke_file->blocks_string,"[]")){
		free(poke_file->blocks);
		poke_file->blocks = string_split("",",");
	}else{
		char* _blocks_string = string_substring(poke_file->blocks_string,1,strlen(poke_file->blocks_string)-2);
		free(poke_file->blocks);
		poke_file->blocks = string_split(_blocks_string,",");
		free(_blocks_string);
	}

	free(poke_file->size);
	poke_file->size = string_itoa(tamanio);

	return 1;
}

t_list* obtener_posiciones_pokemon_dos(file_pokemon* poke_file) {
	t_list* posiciones = list_create();

	char** bloques_array = poke_file->blocks;
	t_list* renglones = obtener_posiciones_de_bloques(bloques_array);

	for(int i = 0 ; i < list_size(renglones) ; i++){
		info_posicion* renglon = list_get(renglones, i);
		t_posicion* posicion = malloc(sizeof(t_posicion));
		posicion->posicionX = renglon->posicion.posicionX;
		posicion->posicionY = renglon->posicion.posicionY;
		list_add(posiciones, posicion);
	}
	list_destroy_and_destroy_elements(renglones,free);
	return posiciones;
}

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

char* pokemon_file_create(char* especie) {

	char* directorio_files = obtener_directorio_files();
	char* path_pokemon = path(directorio_files, especie);

	printf("Creo la carpeta de la especie %s\n",especie);

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
