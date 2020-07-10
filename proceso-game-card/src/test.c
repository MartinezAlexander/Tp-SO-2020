#include "test.h"

void test_agregar_cantidad_a_archivo() {
	//Creo archivo pikachu (solo primera vez)
	//pokemon_file_create("Pikachu");

	char* file = "/home/utnso/Escritorio/tall-grass/Files/Pikachu/Metadata.bin";

	//6 bytes
	agregar_pokemon(file, posicion_create(3, 3), 10);
	//11 bytes
	agregar_pokemon(file, posicion_create(7, 0), 4);
	//18 bytes
	agregar_pokemon(file, posicion_create(1, 2), 150);
	//26 bytes
	agregar_pokemon(file, posicion_create(8, 3), 3850);
	//34 bytes
	agregar_pokemon(file, posicion_create(1, 0), 4102);
	//43 bytes
	agregar_pokemon(file, posicion_create(10, 7), 1245);
	//52 bytes
	agregar_pokemon(file, posicion_create(18, 3), 2140);
	//60 bytes
	agregar_pokemon(file, posicion_create(1, 4), 1798);

	//5 bytes en nuevo archivo
	agregar_pokemon(file, posicion_create(8, 8), 4);

	//0 bytes en primer bloque
	agregar_pokemon(file, posicion_create(3, 3), 4);

	/*
	 * Comenzando con bloques vacios, deberian guardarse los primers 60 bytes en el primer bloque.
	 * Luego los proximos 5 bytes en uno nuevo.
	 * Y el ultimo agregar modificaria el valor de cantidad de una linea del primer bloque (0 bytes).
	 */
}

/*
 Los siguientes test sirven si se utilizan solo con la carpeta Metadata creada y la
 Metadata.bin deben estar configurados de la siguiente manera:

 BLOCK_SIZE=16
 BLOCKS=5192
 MAGIC_NUMBER=TALL_GRASS

 */

void crear_bloques_con_filesystem_vacio() {
	t_posicion posicion;
	posicion.posicionX = 5;
	posicion.posicionY = 6;
	int cantidad = 10;
	int bloque_inicial = 5;

	int bloque_nuevo = agregar_registro_a_bloque(posicion, cantidad,
			bloque_inicial);
	if (bloque_nuevo != -1) {
		printf("Tengo asignado el bloque 5 y el bloque %d\n", bloque_nuevo);
		bloque_inicial = bloque_nuevo;
	} else {
		puts("Tengo asignado el bloque 5");
	}

	posicion.posicionX = 8;
	posicion.posicionY = 8;
	cantidad = 4;

	bloque_nuevo = agregar_registro_a_bloque(posicion, cantidad,
			bloque_inicial);
	if (bloque_nuevo != -1) {
		printf("Tengo asignado el bloque 5 y el bloque %d\n", bloque_nuevo);
		bloque_inicial = bloque_nuevo;
	} else {
		puts("Tengo asignado el bloque 5");
	}

	posicion.posicionX = 11;
	posicion.posicionY = 11;
	cantidad = 20;

	bloque_nuevo = agregar_registro_a_bloque(posicion, cantidad,
			bloque_inicial);
	if (bloque_nuevo != -1) {
		printf("Tengo asignado el bloque 5 y el bloque %d\n", bloque_nuevo);
		bloque_inicial = bloque_nuevo;
	} else {
		puts("Tengo asignado el bloque 5");
	}

	posicion.posicionX = 2;
	posicion.posicionY = 3;
	cantidad = 4;

	bloque_nuevo = agregar_registro_a_bloque(posicion, cantidad,
			bloque_inicial);
	if (bloque_nuevo != -1) {
		printf("Tengo asignado el bloque 5 y el bloque %d\n", bloque_nuevo);
		bloque_inicial = bloque_nuevo;
	} else {
		puts("Tengo asignado el bloque 5");
	}
}

// Primero utilizar una unica vez la funcion crear bloques con filesystem vacio
void leer_posiciones_de_bloques_con_filesystem_inicializado() {

	char* bloques = "5-0";
	char** bloques_asignados = string_split(bloques, "-");

	t_list* posiciones_en_bloque = obtener_posiciones_de_bloques(
			bloques_asignados);

	puts("Posiciones leidas de los bloques 5 y 0");
	for (int i = 0; i < list_size(posiciones_en_bloque); i++) {
		info_posicion* posicion = list_get(posiciones_en_bloque, i);
		info_posicion_mostrar(posicion);
	}
}

// Primero utilizar una unica vez la funcion crear bloques con filesystem vacio
void decrementar_en_uno_5_6_y_11_11() {

	char* bloques = "5-0";
	char** bloques_asignados = string_split(bloques, "-");

	t_list* posiciones_en_bloque = obtener_posiciones_de_bloques(
			bloques_asignados);

	info_posicion* posicion_5_6 = list_get(posiciones_en_bloque, 0);
	info_posicion* posicion_11_11 = list_get(posiciones_en_bloque, 2);

	puts("Posiciones (5,6) y (11,11) antes de decrementarlos");
	// cantidad en 5 6 = 10 y en 11 11 = 20
	info_posicion_mostrar(posicion_5_6);
	info_posicion_mostrar(posicion_11_11);

	posicion_5_6->cantidad--;
	posicion_11_11->cantidad--;

	puts("Posiciones (5,6) y (11,11) despues de decrementarlos");
	// cantidad en 5 6 = 9 y en 11 11 = 19
	info_posicion_mostrar(posicion_5_6);
	info_posicion_mostrar(posicion_11_11);

	actualizar_bloques(bloques_asignados, posiciones_en_bloque);
}

void elimino_posicion_11_11() {
	char* bloques = "5-0";
	char** bloques_asignados = string_split(bloques, "-");

	t_list* posiciones_en_bloque = obtener_posiciones_de_bloques(
			bloques_asignados);

	info_posicion* posicion1_1 = list_remove(posiciones_en_bloque, 2);
	puts("Posicion eliminada");
	info_posicion_mostrar(posicion1_1);

	actualizar_bloques(bloques_asignados, posiciones_en_bloque);
}

void elimino_posicion_11_11_y_2_3_para_liberar_bloque_0() {
	char* bloques = "5-0";
	char** bloques_asignados = string_split(bloques, "-");

	t_list* posiciones_en_bloque = obtener_posiciones_de_bloques(
			bloques_asignados);

	info_posicion* posicion1_1 = list_remove(posiciones_en_bloque, 2);
	puts("Posicion eliminada");
	info_posicion_mostrar(posicion1_1);

	info_posicion* posicion_2_3 = list_remove(posiciones_en_bloque, 2);
	puts("Posicion eliminada");
	info_posicion_mostrar(posicion_2_3);

	actualizar_bloques(bloques_asignados, posiciones_en_bloque);
}
