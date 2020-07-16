/*
 * proceso-game-card.h
 *
 *  Created on: 6 jun. 2020
 *      Author: utnso
 */

#ifndef PROCESO_GAME_CARD_H_
#define PROCESO_GAME_CARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/txt.h>
#include <commons/bitarray.h>
#include "conexion_gameboy.h"
#include "variables_globales.h"
#include "conexion_broker.h"
#include "tall_grass.h"

void inicializar_variables_globales();

t_config* leer_config(void);

void inicializar_filesystem();

void terminar_programa();

#endif /* PROCESO_GAME_CARD_H_ */
