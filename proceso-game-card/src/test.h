/*
 * test.h
 *
 *  Created on: 8 jul. 2020
 *      Author: utnso
 */

#ifndef TEST_H_
#define TEST_H_

#include "file_pokemon.h"
#include "block.h"
#include <commons/collections/list.h>
#include "info_posiciones.h"
#include <commons/string.h>

void test_agregar_cantidad_a_archivo();

void crear_bloques_con_filesystem_vacio();

void leer_posiciones_de_bloques_con_filesystem_inicializado();

void decrementar_en_uno_5_6_y_11_11();

void elimino_posicion_11_11();

void elimino_posicion_11_11_y_2_3_para_liberar_bloque_0();

#endif /* TEST_H_ */
