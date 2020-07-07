/*
 * block.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "block.h"

//TODO: Refactor -> Borrar lista de bloques, los bloques los accedo directamente a partir del indice.
void crear_block(int numero){
	block* bloque = malloc(sizeof(block));

	char* nombre_archivo = string_itoa(numero);
	string_append(&nombre_archivo, ".bin");
	char* ruta_directorio = path(punto_de_montaje_tallgrass, "Blocks");

	bloque->path = path(ruta_directorio, nombre_archivo);
	bloque->numero = numero;
	bloque->datos = crear_archivo(ruta_directorio, nombre_archivo);
	free(nombre_archivo);
	free(ruta_directorio);

	ocupar_bloque(numero);
	list_add(bloques, bloque);
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



