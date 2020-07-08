/*
 * block.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "block.h"

//TODO ver donde repetimos codigo o no usamos funciones de cuando mergeamos con lo de ale

char* crear_nombre_bloque_numero(int numero){
	char* nombre_bloque = NULL;

	if(numero >= 0 && numero < blocks){
		nombre_bloque = string_itoa(numero);
		string_append(&nombre_bloque, ".bin");
	}

	return nombre_bloque;
}

char* crear_directorio_bloque(){
	return path(punto_de_montaje_tallgrass, "Blocks");
}

t_config* crear_block(int numero){

	char* directorio_bloques = crear_directorio_bloque();
	char* nombre_bloque = crear_nombre_bloque_numero(numero);
	t_config* bloque = NULL;

	if(nombre_bloque != NULL){
		FILE* bloque_file = crear_archivo(directorio_bloques, nombre_bloque);
		if(bloque_file != NULL){
			bloque = config_create(path(directorio_bloques, nombre_bloque));
			ocupar_bloque(numero);
			fclose(bloque_file);
		}
	}

	free(nombre_bloque);
	free(directorio_bloques);
	return bloque;
}

t_config* obtener_bloque_por_indice(int numero_bloque){
	char* directorio_bloques = crear_directorio_bloque();
	char* nombre_bloque = crear_nombre_bloque_numero(numero_bloque);
	t_config* bloque = NULL;

	if(nombre_bloque != NULL){
		//crear_archivo(directorio_bloques, nombre_bloque);
		bloque = config_create(path(directorio_bloques,nombre_bloque));
	}

	free(nombre_bloque);
	free(directorio_bloques);

	return bloque;
}

int bloque_esta_libre(int numero_bloque){
	int libre = -1;

	if(numero_bloque >= 0 && numero_bloque < blocks && estado_bloques->bitarray[numero_bloque]=='0'){
		libre = 1;
	}else{
		libre = 0;
	}

	return libre;
}

int es_un_caracter_valido(char caracter){
	int valido = 0;
	if(caracter > 47 && caracter < 58){
		valido = 1;
	}
	if(caracter == 45){
		valido = 1;
	}
	if(caracter == 61){
		valido = 1;
	}
	return valido;
}

int obtener_tamanio_ocupado_por_bloque(int numero_bloque){
	int tamanio = 0;

	if(!bloque_esta_libre(numero_bloque)){

		char* path_bloque = path(crear_directorio_bloque(),crear_nombre_bloque_numero(numero_bloque));
		FILE* bloque = fopen(path_bloque, "r");

		if(bloque != NULL){
			char caracter = fgetc(bloque);
			while(caracter != EOF){
				if(es_un_caracter_valido(caracter)){
					tamanio++;
				}
				caracter = fgetc(bloque);
			}

			fclose(bloque);
		}
	}

	return tamanio;
}

int bytes_libres_bloque(int numero_bloque){
	return block_size - obtener_tamanio_ocupado_por_bloque(numero_bloque);
}

int entra_en_bloque(t_posicion posicion,int cantidad, int numero_bloque){
	char* info_cantidad = string_from_format("%d-%d=%d",posicion.posicionX,posicion.posicionY,cantidad);
	int tamanio_entrada = string_length(info_cantidad);
	return bytes_libres_bloque(numero_bloque) >= tamanio_entrada;
}

/*
 * Devuelve el bloque del listado que tiene la posicion dada y tiene lugar para sumarle la cantidad.
 * En caso de que no exista o no tenga lugar devuelve -1.
 */
int obtener_bloque_con_posicion(char** bloques, t_posicion posicion, int cantidad){
	char* posicion_string = posicion_to_key(posicion);

	int index = 0;
	while(bloques[index] != NULL){
		int bloque = atoi(bloques[index]);

		t_config* config_bloque = obtener_bloque_por_indice(bloque);

		if(config_has_property(config_bloque, posicion_string)){
			int espacio_libre = block_size - obtener_tamanio_ocupado_por_bloque(bloque);

			int cantidad_en_posicion = config_get_int_value(config_bloque, posicion_string);

			char* cantidad_vieja = string_itoa(cantidad_en_posicion);
			char* cantidad_nueva = string_itoa(cantidad_en_posicion + cantidad);

			int espacio_necesario = string_length(cantidad_nueva) - string_length(cantidad_vieja);

			free(cantidad_vieja);
			free(cantidad_nueva);

			if(espacio_necesario < espacio_libre){
				free(posicion_string);
				config_destroy(config_bloque);

				return bloque;
			}
		}

		config_destroy(config_bloque);

		index++;
	}

	return  -1;
}

char* posicion_to_key(t_posicion posicion){
	return string_from_format("%d-%d", posicion.posicionX, posicion.posicionY);
}

/*
 * Devuelve el primer bloque del listado con espacio suficiente para agregar una entrada del tipo
 * 'x-y=cantidad'.
 * Devuelve -1 en caso de que ninguno tenga lugar
 */
int obtener_primer_bloque_con_espacio(char** bloques, t_posicion posicion, int cantidad){
	int index = 0;
	while(bloques[index] != NULL){
		int bloque = atoi(bloques[index]);

		int espacio_libre = block_size - obtener_tamanio_ocupado_por_bloque(bloque);

		char* nueva_entrada = string_from_format("%d-%d=%d", posicion.posicionX, posicion.posicionY, cantidad);
		int tamanio_nueva_entrada = string_length(nueva_entrada);
		free(nueva_entrada);

		if(tamanio_nueva_entrada < espacio_libre) return bloque;

		index++;
	}

	return -1;
}

void agregar_nuevo_pokemon_a_bloque(int bloque, t_posicion posicion, int cantidad){
	char* posicion_string = posicion_to_key(posicion);
	char* nueva_cantidad = string_itoa(cantidad);

	t_config* config_bloque = obtener_bloque_por_indice(bloque);

	config_set_value(config_bloque, posicion_string, nueva_cantidad);
	config_save(config_bloque);
	config_destroy(config_bloque);

	free(nueva_cantidad);
	free(posicion_string);
}


void agregar_pokemon_a_bloque(int bloque, t_posicion posicion, int cantidad){
	char* posicion_string = posicion_to_key(posicion);

	t_config* config_bloque = obtener_bloque_por_indice(bloque);
	int cantidad_actual = config_get_int_value(config_bloque, posicion_string);

	char* nueva_cantidad = string_itoa(cantidad_actual + cantidad);
	config_set_value(config_bloque, posicion_string, nueva_cantidad);
	config_save(config_bloque);
	config_destroy(config_bloque);

	free(nueva_cantidad);
	free(posicion_string);
}

/*
 * Decrementa en uno la cantidad de pokemon para la posicion dada.
 * Devuelve 1 en caso de que haya eliminado la fila debido a que la cantidad quedo en 0.
 */
int eliminar_pokemon_de_bloque(int bloque, t_posicion posicion){
	char* posicion_string = posicion_to_key(posicion);

	t_config* config_bloque = obtener_bloque_por_indice(bloque);
	int cantidad_actual = config_get_int_value(config_bloque, posicion_string);

	int eliminar_fila = cantidad_actual - 1 <= 0;

	if(eliminar_fila){
		config_remove_key(config_bloque, posicion_string);
	}else{
		char* nueva_cantidad = string_itoa(cantidad_actual - 1);
		config_set_value(config_bloque, posicion_string, nueva_cantidad);
		free(nueva_cantidad);
	}

	config_save(config_bloque);
	config_destroy(config_bloque);
	free(posicion_string);

	return eliminar_fila;
}

/*
 * Retorna un listado con todas las posiciones que posee un bloque
 */
t_list* obtener_posiciones_de_bloque(int bloque){
	t_list* posiciones = list_create();

	char* path_bloque = path(crear_directorio_bloque(),crear_nombre_bloque_numero(bloque));
	FILE* archivo_bloque = fopen(path_bloque, "r");

	char caracter = 'x';

	char* posicion_x = string_new();
	char* posicion_y = string_new();

	int estado_lectura = 1;

	while(caracter != EOF){
		caracter = fgetc(archivo_bloque);

		if(caracter == '='){
			estado_lectura = 3;
			continue;
		}

		if(caracter == '-'){
			estado_lectura = 2;
			continue;
		}

		if(caracter == '\n'){
			estado_lectura = 1;
			posicion_x = string_new();
			posicion_y = string_new();
			continue;
		}

		if(estado_lectura == 1){
			string_append_with_format(&posicion_x, "%c", caracter);
		}

		if(estado_lectura == 2){
			string_append_with_format(&posicion_y, "%c", caracter);
		}

		if(estado_lectura == 3){
			estado_lectura = 4;

			t_posicion* posicion = malloc(sizeof(t_posicion));
			posicion->posicionX = atoi(posicion_x);
			posicion->posicionY = atoi(posicion_y);
			list_add(posiciones, posicion);
		}
	}

	fclose(archivo_bloque);

	return posiciones;
}
