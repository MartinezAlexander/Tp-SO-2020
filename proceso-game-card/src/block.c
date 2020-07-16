/*
 * block.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "block.h"

char* crear_nombre_bloque_numero(int numero) {
	char* nombre_bloque = NULL;

	if (numero >= 0 && numero < blocks) {
		nombre_bloque = string_itoa(numero);
		string_append(&nombre_bloque, ".bin");
	}

	return nombre_bloque;
}

char* crear_directorio_bloque() {
	char* directorio_bloque = obtener_directorio_blocks();

	if (directorio_bloque == NULL) {
		crear_directorio(punto_de_montaje_tallgrass, "Blocks");
		directorio_bloque = obtener_directorio_blocks();
	}
	return directorio_bloque;
}

void crear_block(int numero) {

	char* directorio_bloques = crear_directorio_bloque();
	char* nombre_bloque = crear_nombre_bloque_numero(numero);

	if (nombre_bloque != NULL) {
		FILE* bloque_file = crear_archivo(directorio_bloques, nombre_bloque);
		if (bloque_file != NULL) {
			ocupar_bloque(numero);
			fclose(bloque_file);
		}
	}

	free(nombre_bloque);
	free(directorio_bloques);
}


char* leer_bloque(int bloque) {
	char* path_blocks = obtener_directorio_blocks();
	char* nombre_bloque = crear_nombre_bloque_numero(bloque);
	char* path_bloque = path(path_blocks, nombre_bloque);

	FILE* fd = fopen(path_bloque, "r");
	char caracter = fgetc(fd);
	char* posiciones = string_new();
	while (caracter != EOF) {
		char* caracter_aux = string_repeat(caracter,1);
		string_append(&posiciones,caracter_aux);
		//posiciones = string_from_format("%s%c", posiciones, caracter);
		caracter = fgetc(fd);
		free(caracter_aux);
	}
	fclose(fd);

	free(path_blocks);
	free(path_bloque);
	free(nombre_bloque);
	return posiciones;
}

t_list* obtener_posiciones_de_bloques(char** bloques) {
	char* posiciones = string_new();

	int i = 0;

	while (bloques[i] != NULL) {

		int bloque = atoi(bloques[i]);
		char* posiciones_por_bloque = leer_bloque(bloque);
		string_append(&posiciones, posiciones_por_bloque);
		free(posiciones_por_bloque);
		i++;
	}
	return convertir_info_posiciones(posiciones);
}

int array_cantidad_de_elementos(char** array) {
	int cantidad = 0;
	while (array[cantidad] != NULL)
		cantidad++;
	return cantidad;
}

char* obtener_path_bloque_de_lista(char** bloques, int indice_numero_bloque) {
	char* path_blocks = obtener_directorio_blocks();
	char* nombre_bloque = NULL;
	char* path_bloque = NULL;

	if(path_blocks == NULL){
		crear_directorio(punto_de_montaje_tallgrass,"Blocks");
		path_blocks = path(punto_de_montaje_tallgrass, "Blocks");
	}

	if (indice_numero_bloque >= 0 && indice_numero_bloque < array_cantidad_de_elementos(bloques)) {
		int num_bloque = atoi(bloques[indice_numero_bloque]);
		nombre_bloque = crear_nombre_bloque_numero(num_bloque);
		path_bloque = path(path_blocks, nombre_bloque);
	}

	free(path_blocks);
	free(nombre_bloque);
	return path_bloque;
}

void vaciar_bloques(char** bloques) {
	int i = 0;
	while (bloques[i] != NULL) {
		char* path_bloque = obtener_path_bloque_de_lista(bloques, i);
		FILE* fd = fopen(path_bloque, "w");
		fclose(fd);
		i++;
		free(path_bloque);
	}
}

/*
 * Actualiza los bloques con el  listado dado, y en caso de que ocupen menos lugar que antes
 * libera los ultimos bloques y devuelve cuantos bloques libero
 */
int actualizar_bloques(char** bloques, t_list* posiciones) {
	vaciar_bloques(bloques);

	char* posiciones_string = lista_info_posicion_to_string(posiciones);
	int indice_numero_bloque = 0;
	int cantidad_escrita = 0;
	char* path_bloque = obtener_path_bloque_de_lista(bloques,indice_numero_bloque);
	int cantidad_bloques_usados = 1;
	FILE* bloque = fopen(path_bloque, "a");

	for (int i = 0; i < string_length(posiciones_string); i++) {
		if (cantidad_escrita == block_size) {
			free(path_bloque);
			indice_numero_bloque++;
			cantidad_escrita = 0;
			fclose(bloque);
			path_bloque = obtener_path_bloque_de_lista(bloques,indice_numero_bloque);
			bloque = fopen(path_bloque, "a");
			cantidad_bloques_usados++;
		}
		fputc(posiciones_string[i], bloque);
		cantidad_escrita++;
	}
	free(path_bloque);

	if(string_length(posiciones_string) == 0){
		cantidad_bloques_usados = 0;
	}

	fclose(bloque);

	int cant_liberados = array_cantidad_de_elementos(bloques) - cantidad_bloques_usados;

	if (cant_liberados > 0) {
		for (int i = 0; i < cant_liberados; i++) {
			int bloque = atoi(bloques[cantidad_bloques_usados - i]);
			liberar_bloque(bloque);
		}
	}
	free(posiciones_string);
	return cant_liberados;
}

