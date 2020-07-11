/*
 * tall_grass.h
 *
 *  Created on: 6 jun. 2020
 *      Author: utnso
 */

#ifndef TALL_GRASS_H_
#define TALL_GRASS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/collections/list.h>
#include <commons/txt.h>
#include <commons/config.h>
#include <mensajes/posiciones.h>
#include <dirent.h>
#include <pthread.h>
#include "variables_globales.h"

pthread_mutex_t mutex_listar_directorios;

/*
Crea un archivo con el nombre (char* archivo) en el directorio indicado por (char* directorio)
*/
FILE* crear_archivo(char* en_donde_crear, char* nombre_archivo);

/*
Crea un directorio con un nombre especifico en donde se le indique
*/
void crear_directorio(char* en_donde_crear, char* nombre_directorio);

/*
Concatena dos path colocandole una / entre los dos
*/
char* path(char* direccion, char* direccion2);

/*
Dada una ruta absoluta de un directorio, crea un lista con los nombre
de su contenido (tanto archivos, como directorios)
*/
t_list* listar_directorio(char* path);

/*
Se fija si un archivo/directorio se encuentra en el interior del directorio
que se encuentra en la ruta que se pasa por parametro
*/
int existe_archivo_en(char* nombre_archivo,char* path);

/*
Lee la informacion del metadata del filesystem para guardar la informacion
del tamaño de los bloques y la cantidad de bloques que tiene el mismo
*/
void metadata_cargar();

/*
Devuelve el path del directorio donde se encuentran los bloques, o NULL si
el directorio donde deberian estar los bloques no existe.
*/
char* obtener_directorio_blocks();

/*
Devuelve el path del directorio donde se encuentran los files, o NULL si
el directorio donde deberian estar los pokemon files no existe.
*/
char* obtener_directorio_files();

/*
Devuelve el path del directorio donde se encuentran el metadata, o NULL si
el directorio donde deberian estar el metadata no existe.
*/
char* obtener_directorio_metadata();

/*
Devuelve el path del directorio donde se encuentran el bitmap, o NULL si
el directorio donde deberian estar el bitmap no existe.
*/
char* obtener_directorio_bitmap();


#endif /* TALL_GRASS_H_ */
