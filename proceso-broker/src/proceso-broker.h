
#ifndef PROCESO_BROKER_H_
#define PROCESO_BROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<mensajes/server.h>
#include<mensajes/mensajes.h>
#include<mensajes/localized_pokemon.h>
#include<mensajes/appeared_pokemon.h>
#include<mensajes/get_pokemon.h>
#include<mensajes/catch_pokemon.h>
#include<mensajes/caught_pokemon.h>


//estructura de cola con sus respectivos suscriptores
typedef struct{
	t_queue* queue;
	t_list* suscriptores;
}t_cola_mensajeria;

typedef struct{
	uint32_t id;
	uint32_t id_correlativo;
	void* mensaje;
}t_mensaje_id;


t_cola_mensajeria* cola_mensajeria_create();
t_log* iniciar_logger(char* path);
t_config* leer_config(void);

#endif /* PROCESO_BROKER_H_ */
