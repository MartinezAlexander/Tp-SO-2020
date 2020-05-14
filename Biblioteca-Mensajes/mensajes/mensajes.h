#ifndef MENSAJES_H
#define MENSAJES_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum{
	BROKER = 1,
	TEAM,
	GAMECARD,
	SUSCRIPTOR,
	GAMEBOY
}t_proceso;

typedef enum
{
	NEW_POKEMON = 1,
    LOCALIZED_POKEMON,
    GET_POKEMON,
    APPEARED_POKEMON,
    CATCH_POKEMON,
    CAUGHT_POKEMON,
    SUSCRIPCION
}op_code;

typedef struct{
	int size;
	void* stream;
} t_buffer;

typedef struct{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


typedef struct{
	int32_t id;
	int32_t id_correlativo;
	op_code codigo;
	void* mensaje;
}t_mensaje;

/*
crea un void* de la siguiente forma
[codigo_operacion][tamanio_de_buffer][buffer]
y lo retorna, ademas indica la cantidad de bytes
que ocupa todo el stream anteriormente nombrado
*/
void* serializar_paquete(t_paquete* paquete, int *bytes);

/*
recibe el buffer que fue creado a traves de un protocolo el cual depende 
de un tipo de codigo de operacion, por lo cual para indicar de que tipo
de protocolo se utilizo se debe crear el paquete que consiste en una
estructura donde un campo es el codigo y el otro el buffer por lo cual
en este metodo se crea dichp paquete.
*/
t_paquete* empaquetar_buffer(t_buffer* buffer, op_code codigo);

/*
enviar_mensaje es un metodo que envia todos los mensajes incluidos en el ENUM
a traves de un socket que esta conectado a un server para lo cual necesita un
t_mensaje* y el socket a traves del cual enviara el mensaje.
ej:
t_new_pokemon* pokemon = new_pokemon_create("pikachu",2,3,4);
t_mensaje* mensaje = mensaje_simple_create((void*)pokemon,NEW_POKEMON);
enviar_mensaje(mensaje,socket);
*/
void enviar_mensaje(t_mensaje* mensaje, int socket_cliente);

/*
recibir_mensaje es un metodo que se encarga de recibir los mensajes que lleguen
a un socket especifico y lo interpreta deserializandolo a traves del protocolo
que escoge luego de haber leido el codigo de operacion que recibio, una vez hecho 
esto informa que tipo de mensaje recibio a traves de codigo_operacion y retorna 
el mensaje de una forma generica t_mensaje*.
ej:
t_mensaje* mensaje = recibir_mensaje(socket);
*/
t_mensaje* recibir_mensaje(int socket_cliente);

/*
mensaje_simple_create recibe cualquier tipo de mensaje y un codigo para crear un
t_mensaje para poder enviarlo a traves de un socket
*/
t_mensaje* mensaje_simple_create(void* mensaje, op_code codigo);

/*
mensaje_con_id_create recibe cualquier tipo de mensaje, su codigo y el id del mensaje
para crear un t_mensaje para poder enviarlo a traves de un socket
*/
t_mensaje* mensaje_con_id_create(void* mensaje, op_code codigo, int32_t id);

/*
mensaje_con_id_correlativo_create recibe cualquier tipo de mensaje, su codigo y el
id del mensaje, mas su id correlativo para crear un t_mensaje para poder enviarlo a
traves de un socket
*/
t_mensaje* mensaje_con_id_correlativo_create(void* mensaje, op_code codigo, int32_t id_c);

/*
mensaje_obtener_id recibe un t_mensaje y le devuelve el id que posee, en caso de no
tener retorna -1
*/
int32_t mensaje_obtener_id(t_mensaje* mensaje);

/*
mensaje_obtener_id_correlativo recibe un t_mensaje y devuelve el id correlativo que
posee o -1 si no tiene ninguno
*/
int32_t mensaje_obtener_id_correlativo(t_mensaje* mensaje);

/*
mensaje_obtener_codigo dado un t_mensaje te devuelve un codigo de mensaje para que
saber como interpreatar el contenido del mensaje
*/
op_code mensaje_obtener_codigo(t_mensaje* mensaje);

/*
mensaje_obtener_contenido dado un t_mensaje te devuelve un void* con el contenido
del mensaje el cual debera ser casteado en el exterior dependiendo el codigo que
tenga asignado el mensaje
*/
void* mensaje_obtener_contenido(t_mensaje* mensaje);

/*
mensaje_destroy libera la memoria del mensaje
*/
void mensaje_destroy(t_mensaje* mensaje);

#endif 
