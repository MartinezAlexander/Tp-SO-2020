/*
 * bitmap.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */
#include "bitmap.h"

void bitmap_cargar(){

	estado_bloques = malloc(sizeof(bitmap));
	int bitarray_size = blocks / 8;
	char* bits = malloc(bitarray_size);
	estado_bloques->bitarray = bitarray_create_with_mode(bits,bitarray_size,LSB_FIRST);

	for (int i = 0; i < blocks; i++){
		bitarray_clean_bit(estado_bloques->bitarray,i);
	}

	char* directorio_metadata = obtener_directorio_metadata();
	if (!existe_archivo_en("Bitmap.bin",directorio_metadata)) {
		estado_bloques->path = obtener_directorio_bitmap();

		FILE* fd = fopen(estado_bloques->path,"a");

		if (fd != NULL) {
			for (int i = 0; i < bitarray_size; i++) {
				fputc(estado_bloques->bitarray->bitarray[i], fd);
			}
		}

		fclose(fd);
		int file_descriptor = open(estado_bloques->path, O_RDWR);
		estado_bloques->bitarray->bitarray = mmap(NULL,bitarray_size,PROT_WRITE,MAP_SHARED,file_descriptor,0);

	}else{
		estado_bloques->path = obtener_directorio_bitmap();
		int file_descriptor = open(estado_bloques->path, O_RDWR);
		estado_bloques->bitarray->bitarray = mmap(NULL,bitarray_size,PROT_WRITE,MAP_SHARED,file_descriptor,0);
	}
}

void ocupar_bloque(int nuevo_bloque){
	if(nuevo_bloque >= 0 && nuevo_bloque < blocks){
		bitarray_set_bit(estado_bloques->bitarray,nuevo_bloque);
	}else{
		printf("[Bloque] Error al ocupar bloque, no existe el bloque %d\n", nuevo_bloque);
	}
}

void liberar_bloque(int numero_bloque){
	if(numero_bloque >= 0 && numero_bloque < blocks){
		bitarray_clean_bit(estado_bloques->bitarray, numero_bloque);
	}else{
		printf("[Bloque] Error al liberar bloque, no existe el bloque %d\n", numero_bloque);
	}
}

int obtener_bloque_disponible(){
	int i = 0;
	while(bitarray_test_bit(estado_bloques->bitarray,i)){
		i++;
	}
	return i;
}


