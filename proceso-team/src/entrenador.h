/*
 * entrenador.h
 *
 *  Created on: 1 may. 2020
 *      Author: utnso
 */

#ifndef PROCESO_TEAM_SRC_ENTRENADOR_H_
#define PROCESO_TEAM_SRC_ENTRENADOR_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<commons/string.h>
#include<commons/config.h>


typedef enum{
	NEW, READY, EXEC, BLOCKED, EXIT
} estado_planificacion;


/* Habria que importarla de mensajes*/
typedef struct{
	uint32_t x;
	uint32_t y;
} t_posicion;

typedef struct{
	t_posicion posicion;
	estado_planificacion estado;
	char** objetivos;
	char** pokemones_adquiridos;

} t_entrenador;

/*
typedef struct{
	char* pokemon;
	uint32_t cantidad;
} t_objetivo;
*/


t_entrenador* entrenador_create(char* posicion, char* pokemones, char* objetivos);

void entrenador_mostrar(t_entrenador* entrenador);


#endif /* PROCESO_TEAM_SRC_ENTRENADOR_H_ */
