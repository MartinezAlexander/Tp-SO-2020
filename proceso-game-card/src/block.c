/*
 * block.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "block.h"

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
