#ifndef MENSAJES_H
#define MENSAJES_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum
{
	NEW_POKEMON = 1,
    LOCALIZED_POKEMON,
    GET_POKEMON,
    APPEARED_POKEMON,
    CATCH_POKEMON,
    CAUGHT_POKEMON,
    MENSAJE
}op_code;

typedef struct{
	int size;
	void* stream;
} t_buffer;

typedef struct{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


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
a traves de un socket que esta conectado a un server para lo cual necesita el
mensaje casteado a (void*), el codigo de operacion para saber que protocolo 
utilizar a la hora de serializar el mensaje y por ultimo el socket a traves del
cual enviara el mensaje.
ej: enviar_mensaje((t_new_pokemon*)new_mensaje,NEW_POKEMON,socket);
*/
void enviar_mensaje(void* mensaje,op_code codigo, int socket_cliente);

/*
recibir_mensaje es un metodo que se encarga de recibir los mensajes que lleguen
a un socket especifico y lo interpreta deserializandolo a traves del protocolo
que escoge luego de haber leido el codigo de operacion que recibio, una vez hecho 
esto informa que tipo de mensaje recibio a traves de codigo_operacion y retorna 
el mensaje de una forma generica por lo cual si alguien lo quiere utilizar debera 
castearlo al tipo de mensaje que corresponda.
ej: op_code codigo; 
void* mensaje = recibir_mensaje(socket,&codigo);
t_new_pokemon* new_mensaje;
if(codigo == NEW_POKEMON){
    new_mensaje = (t_new_pokemon*)mensaje;
}
al final de esto tenemos un mensaje new pokemon listo para utilizar.
*/
void* recibir_mensaje(int socket_cliente, op_code* codigo_operacion);

#endif 
