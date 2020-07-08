#include "test.h"

void test_agregar_cantidad_a_archivo(){
	//Creo archivo pikachu (solo primera vez)
	//pokemon_file_create("Pikachu");

	char* file = "/home/utnso/Escritorio/tall-grass/Files/Pikachu/Metadata.bin";


	//6 bytes
	agregar_pokemon(file, posicion_create(3,3), 10);
	//11 bytes
	agregar_pokemon(file, posicion_create(7,0), 4);
	//18 bytes
	agregar_pokemon(file, posicion_create(1,2), 150);
	//26 bytes
	agregar_pokemon(file, posicion_create(8,3), 3850);
	//34 bytes
	agregar_pokemon(file, posicion_create(1,0), 4102);
	//43 bytes
	agregar_pokemon(file, posicion_create(10,7), 1245);
	//52 bytes
	agregar_pokemon(file, posicion_create(18,3), 2140);
	//60 bytes
	agregar_pokemon(file, posicion_create(1,4), 1798);

	//5 bytes en nuevo archivo
	agregar_pokemon(file, posicion_create(8,8), 4);

	//0 bytes en primer bloque
	agregar_pokemon(file, posicion_create(3,3), 4);

	/*
	 * Comenzando con bloques vacios, deberian guardarse los primers 60 bytes en el primer bloque.
	 * Luego los proximos 5 bytes en uno nuevo.
	 * Y el ultimo agregar modificaria el valor de cantidad de una linea del primer bloque (0 bytes).
	 */
}
