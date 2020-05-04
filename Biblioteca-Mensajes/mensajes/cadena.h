#ifndef CADENA_H
#define CADENA_H

#include "mensajes.h"

/*
cadena_codigo te retorna el codigo de operacion cuando recibe un cadena
*/
op_code cadena_codigo(char* catch_pokemon);

/*
cadena_to_buffer convierte un mensaje del tipo cadena en un buffer
*/
t_buffer* cadena_to_buffer(char* mensaje);

/*
cadena_from_buffer convierte un buffer en un mensaje del tipo cadena
*/
char* cadena_from_buffer(t_buffer* buffer);

/*
cadena_mostrar dado un mensaje cadena este metodo lo imprime por consola
*/
void cadena_mostrar(char* cadena);

/*
cadena_destroy libera la memoria utilizada para crear la cadena
*/
void cadena_destroy(char* cadena);

#endif