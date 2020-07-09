/*
 * bitmap.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */
#include "bitmap.h"


void bitmap_cargar(){
	estado_bloques = malloc(sizeof(bitmap));

	// TODO posibilidad de que el bitmap no este pero haya bloques creados
	// TODO cargar bitmap en base a nombre de archivos de bloques
	if (!existe_archivo_en("Bitmap.bin",obtener_directorio_metadata())) {
		estado_bloques->path = obtener_directorio_bitmap();

		FILE* fd = fopen(estado_bloques->path,"a");

		char x = '0';

		if (fd != NULL) {
			for (int i = 0; i < blocks; i++) {
				fputc(x, fd);
			}
		}

		fclose(fd);

	}else{
		estado_bloques->path = obtener_directorio_bitmap();
	}

	int file_descriptor = open(estado_bloques->path, O_RDWR);
	estado_bloques->bitarray = mmap(NULL,blocks,PROT_WRITE,MAP_SHARED,file_descriptor,0);

}

void ocupar_bloque(int nuevo_bloque){
	if(nuevo_bloque >= 0 && nuevo_bloque < blocks){
		estado_bloques->bitarray[nuevo_bloque] = '1';
	}else{
		//TODO Error bloque no existe
	}
}

void liberar_bloque(int numero_bloque){
	if(numero_bloque >= 0 && numero_bloque < blocks){
		estado_bloques->bitarray[numero_bloque] = '0';
	}else{
		//TODO Error bloque no existe
	}
}

int obtener_bloque_disponible(){
	for(int i = 0 ; i < blocks ; i++){
		if(estado_bloques->bitarray[i]=='0') return i;
	}
	printf("No se encontraron bloques disponibles");
	return -1;
	//TODO Si nos dicen que hacer en este caso en el enunciado resolver error.
}
