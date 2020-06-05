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

typedef enum{
	ACK = 1,
	ID,
	OK_SUSCRIPTO
}cod_confirmacion;

/*
crea un void* de la siguiente forma
[codigo_operacion][tamanio_de_buffer][buffer]
y lo retorna, ademas indica la cantidad de bytes
que ocupa el stream anteriormente nombrado
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
t_mensaje* y el socket a traves del cual enviara el mensaje. Si lo pudo mandar
retorna la cantidad de bytes que mando sino pudo enviar devuelve un -1;
ej:
t_new_pokemon* pokemon = new_pokemon_create("pikachu",2,3,4);
t_mensaje* mensaje = mensaje_simple_create((void*)pokemon,NEW_POKEMON);
enviar_mensaje(mensaje,socket);
*/
int enviar_mensaje(t_mensaje* mensaje, int socket_cliente);

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
mensaje_to_string crea un string con el contenido del mensaje para poder loguearlo
*/
char* mensaje_to_string(t_mensaje* mensaje);

/*
mensaje_mostrar muestra por pantalla el contenido del mensaje
*/
void mensaje_mostrar(t_mensaje* mensaje);

/*
mensaje_destroy libera la memoria del mensaje
*/
void mensaje_destroy(t_mensaje* mensaje);

int enviar_confirmacion(int32_t num, cod_confirmacion codigo, int socket);
int32_t recibir_confirmacion(int socket, cod_confirmacion* codigo);
char* op_code_to_string(op_code codigo);

/*
enviar_id envia el valor de un id a traves del socket
devueve -1 o 0 ante un error y cualquier otro valor si pudo ser enviado
*/
int enviar_id(int socket,int32_t id);

/*
recibir_id recibe un id del socket indicado
devuelve -1 si lo que recibio no es un ID sino devuelve el valor del ID
*/
int32_t recibir_id(int socket);

/*
enviar_ACK sirve para confirmar que llego un mensaje a traves de un socket
devuelve -1 o 0 si no pudo enviar la confirmacion de recepcion y cualquier
valor mayor a 0 si pudo enviarlo
*/
int enviar_ACK(int socket);

/*
recibir_ACK recibe un confirmacion de recepcion de mensaje de un proceso
devuelve 0 si lo que recibio no es un ACK o 1 si es un ACK
*/
int recibir_ACK(int socket);

/*
confirmar_suscripcion le aviso a un proceso a traves del socket que su solicitud
de suscripcion ha sido aceptada. SI no se puede enviar devuelve -1 o 0, si se envio
devuelve un valor mayor a 0
*/
int confirmar_suscripcion(int socket);

/*
recibir_confirmacion_suscripcion recibe la confirmacion de suscripcion por parte
de un proceso. En caso de que no sea una confirmacion de suscripcion devuelve un 0
si lo es devuelve un 1
*/
int recibir_confirmacion_suscripcion(int socket);

/*
mensaje_size te devuelve el tamaño en bytes de un mensaje contando los dos ids (id
creado por el broker y id correlativo), el codigo de operacion y el contenido del
mensaje pokemon (new,get,appeared,caught,catch,localized)
*/
int mensaje_size(t_mensaje* mensaje);

/*
mensaje_size_contenido te devuelve el tamaño en bytes del contenido del mensaje, es
decir solo el tamaño del mensaje pokemon (new,get,appeared,caught,catch,localized)
*/
int mensaje_size_contenido(t_mensaje* mensaje);

/*
mensaje_to_stream crea un stream con el contenido de un mensaje
*/
void* mensaje_to_stream(t_mensaje* mensaje);

/*
mensaje_from_stream convierte un stream en un mensaje
*/
t_mensaje* mensaje_from_stream(void* stream, int tamanio_stream);

/*
mensaje_stream_obtener_codigo te devuelve el tipo de mensaje que esta
guardado en un stream especifico
*/
op_code mensaje_stream_obtener_codigo(void* stream);

#endif 
