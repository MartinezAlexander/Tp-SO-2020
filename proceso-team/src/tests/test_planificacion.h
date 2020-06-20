/*
 * test_planificacion.h
 *
 *  Created on: 20 jun. 2020
 *      Author: utnso
 */

#ifndef TESTS_TEST_PLANIFICACION_H_
#define TESTS_TEST_PLANIFICACION_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mensajes/pokemon.h>
#include <commons/collections/list.h>
#include "../entrenador.h"
#include "../planificacion/planificacion.h"

void test_fifo();
void test_round_robin();
void test_sjf_sin_desalojo();
void test_sjf_con_desalojo();

#endif /* TESTS_TEST_PLANIFICACION_H_ */
