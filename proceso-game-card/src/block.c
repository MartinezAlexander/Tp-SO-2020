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

