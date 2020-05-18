#ifndef ID_PROCESO_H
#define ID_PROCESO_H

#include "mensajes.h"

typedef struct{
	t_proceso id;
	int32_t pid;
	op_code cola_suscripcion;
}t_suscripcion;


/*
suscripcion_proceso_create crea un mensaje del tipo suscripcion pidiendo como datos
que tipo de proceso es, el pid del proceso y el codigo de la cola a la que se quiere
suscribir
*/
t_suscripcion* suscripcion_proceso_create(t_proceso id_proceso, int32_t pid, op_code cola);

/*
suscripcion_proceso_codigo te retorna el codigo de operacion cuando recibe t_proceso
*/
op_code suscripcion_proceso_codigo(t_suscripcion* id_proceso);

/*
suscripcion_proceso_to_buffer convierte un mensaje del tipo proceso en un buffer
*/
t_buffer* suscripcion_proceso_to_buffer(t_suscripcion* id_proceso);

/*
suscripcion_proceso_from_buffer convierte un buffer en un mensaje del tipo proceso
*/
t_suscripcion* suscripcion_proceso_from_buffer(t_buffer* buffer);

/*
suscripcion_proceso_mostrar dado un mensaje proceso este metodo lo imprime por consola
*/
void suscripcion_proceso_mostrar(t_suscripcion* proceso);

/*
suscripcion_proceso_to_string convierte una suscripcion en un string para loguearlo
*/
char* suscripcion_proceso_to_string(t_suscripcion* suscripcion);

/*
suscripcion_proceso_destroy libera la memoria utilizada para crear el mensaje de tipo proceso
*/
void suscripcion_proceso_destroy(t_suscripcion* id_proceso);

#endif
