#ifndef ID_PROCESO_H
#define ID_PROCESO_H

#include "mensajes.h"

/*
id_proceso_codigo te retorna el codigo de operacion cuando recibe t_proceso
*/
op_code id_proceso_codigo(t_proceso* id_proceso);

/*
id_proceso_to_buffer convierte un mensaje del tipo proceso en un buffer
*/
t_buffer* id_proceso_to_buffer(t_proceso* id_proceso);

/*
id_proceso_from_buffer convierte un buffer en un mensaje del tipo proceso
*/
t_proceso* id_proceso_from_buffer(t_buffer* buffer);

/*
id_proceso_mostrar dado un mensaje proceso este metodo lo imprime por consola
*/
void id_proceso_mostrar(t_proceso* proceso);

/*
id_proceso_destroy libera la memoria utilizada para crear el mensaje de tipo proceso
*/
void id_proceso_destroy(t_proceso* id_proceso);

#endif
