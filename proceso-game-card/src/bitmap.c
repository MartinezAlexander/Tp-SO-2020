/*
 * bitmap.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */
#include "bitmap.h"


void bitmap_cargar(){
	estado_bloques = malloc(sizeof(bitmap));
	estado_bloques->path = path(punto_de_montaje_tallgrass, "Metadata/Bitmap.bin");

	int file_descriptor = open(estado_bloques->path, O_RDWR);
	estado_bloques->bitarray = mmap(NULL,blocks,PROT_WRITE,MAP_SHARED,file_descriptor,0);

	/*FILE* bitmap = fopen(estado_bloques->path, "r");

	if(bitmap != NULL){
		int bitarray_size = blocks / 8;
		void* puntero_a_bits = malloc(bitarray_size);
		estado_bloques->bitarray = bitarray_create(puntero_a_bits,bitarray_size);

		int index = 0;
		char buffer = fgetc(bitmap);

		while(buffer != EOF){
			if (buffer == '1') {
				bitarray_set_bit(estado_bloques->bitarray, index);
			} else {
				bitarray_clean_bit(estado_bloques->bitarray, index);
			}
			printf("Index: %d, bit = %c\n", index, buffer);
			index++;
			buffer = fgetc(bitmap);
		}

		fclose(bitmap);
	}*/
}

void ocupar_bloque(int nuevo_bloque){
	if(nuevo_bloque >= 0 && nuevo_bloque < blocks){
		estado_bloques->bitarray[nuevo_bloque] = '1';
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
