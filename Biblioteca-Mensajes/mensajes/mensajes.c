#include "mensajes.h"
#include "appeared_pokemon.h"
#include "catch_pokemon.h"
#include "caught_pokemon.h"
#include "get_pokemon.h"
#include "localized_pokemon.h"
#include "new_pokemon.h"
#include <sys/socket.h>
#include <commons/string.h>

#include "suscripcion.h"

t_mensaje* mensaje_simple_create(void* mensaje, op_code codigo){
	t_mensaje* nuevo_mensaje = malloc( sizeof(t_mensaje) );
	nuevo_mensaje->codigo = codigo;
	nuevo_mensaje->mensaje = mensaje;
	nuevo_mensaje->id = (-1);
	nuevo_mensaje->id_correlativo = (-1);
	return nuevo_mensaje;
}

t_mensaje* mensaje_con_id_create(void* mensaje, op_code codigo, int32_t id){
	t_mensaje* nuevo_mensaje = malloc( sizeof(t_mensaje) );
	nuevo_mensaje->codigo = codigo;
	nuevo_mensaje->mensaje = mensaje;
	nuevo_mensaje->id = id;
	nuevo_mensaje->id_correlativo = (-1);
	return nuevo_mensaje;
}

t_mensaje* mensaje_con_id_correlativo_create(void* mensaje, op_code codigo, int32_t id_c){
	t_mensaje* nuevo_mensaje = malloc( sizeof(t_mensaje) );
	nuevo_mensaje->codigo = codigo;
	nuevo_mensaje->mensaje = mensaje;
	nuevo_mensaje->id = (-1);
	nuevo_mensaje->id_correlativo = id_c;
	return nuevo_mensaje;
}

int32_t mensaje_obtener_id(t_mensaje* mensaje){
	return mensaje->id;
}

int32_t mensaje_obtener_id_correlativo(t_mensaje* mensaje){
	return mensaje->id_correlativo;
}

op_code mensaje_obtener_codigo(t_mensaje* mensaje){
	return mensaje->codigo;
}

void* mensaje_obtener_contenido(t_mensaje* mensaje){
	return mensaje->mensaje;
}

void mensaje_destroy(t_mensaje* mensaje){
	switch (mensaje->codigo) {
	case SUSCRIPCION:
		suscripcion_proceso_destroy((t_suscripcion*)mensaje->mensaje);
		break;
	case NEW_POKEMON:
		new_pokemon_destroy((t_new_pokemon*) mensaje->mensaje);
		break;
	case LOCALIZED_POKEMON:
		localized_pokemon_destroy((t_localized_pokemon*) mensaje->mensaje);
		break;
	case GET_POKEMON:
		get_pokemon_destroy((t_get_pokemon*) mensaje->mensaje);
		break;
	case APPEARED_POKEMON:
		appeared_pokemon_destroy((t_appeared_pokemon*) mensaje->mensaje);
		break;
	case CATCH_POKEMON:
		catch_pokemon_destroy((t_catch_pokemon*) mensaje->mensaje);
		break;
	case CAUGHT_POKEMON:
		caught_pokemon_destroy((t_caught_pokemon*) mensaje->mensaje);
		break;
	default:
		break;
	}
	free(mensaje);
}


void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	int size_serializado  = sizeof(paquete->codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size;
	void *buffer = malloc(size_serializado);

	int bytes_escritos = 0;

	memcpy(buffer + bytes_escritos,&(paquete->codigo_operacion),sizeof(paquete->codigo_operacion));
	bytes_escritos += sizeof(paquete->codigo_operacion);

	memcpy(buffer  + bytes_escritos,&(paquete->buffer->size),sizeof(paquete->buffer->size));
	bytes_escritos += sizeof(paquete->buffer->size);

	memcpy(buffer + bytes_escritos,paquete->buffer->stream,paquete->buffer->size);
	bytes_escritos += paquete->buffer->size;

	(*bytes) = size_serializado;
	return buffer;
}

t_paquete* empaquetar_buffer(t_buffer* buffer, op_code codigo){
	t_paquete* paquete = malloc( sizeof(t_paquete) );
	paquete->codigo_operacion = codigo;
	paquete->buffer = buffer;
	return paquete;
}

int enviar_mensaje(t_mensaje* mensaje, int socket_cliente)
{
	t_buffer* buffer;

	switch(mensaje->codigo){
		case SUSCRIPCION:
			buffer = suscripcion_proceso_to_buffer((t_suscripcion*)mensaje->mensaje);
			break;
		case NEW_POKEMON:
			buffer = new_pokemon_to_buffer((t_new_pokemon*) mensaje->mensaje);
			break;
		case LOCALIZED_POKEMON:
			buffer = localized_pokemon_to_buffer((t_localized_pokemon*) mensaje->mensaje);
			break;
		case GET_POKEMON:
			buffer = get_pokemon_to_buffer((t_get_pokemon*) mensaje->mensaje);
			break;
		case APPEARED_POKEMON:
			buffer = appeared_pokemon_to_buffer((t_appeared_pokemon*) mensaje->mensaje) ;
			break;
		case CATCH_POKEMON:
			buffer = catch_pokemon_to_buffer((t_catch_pokemon*) mensaje->mensaje);
			break;
		case CAUGHT_POKEMON:
			buffer = caught_pokemon_to_buffer((t_caught_pokemon*) mensaje->mensaje);
			break;
		default:
			break;
	}

	int nuevo_tamanio = buffer->size + 2 * sizeof(int32_t);
	void* nuevo_stream = malloc(nuevo_tamanio);
	int offset = 0;
	memcpy(nuevo_stream + offset, &(mensaje->id), sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(nuevo_stream + offset, &(mensaje->id_correlativo), sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(nuevo_stream + offset, buffer->stream, buffer->size);
	offset += buffer->size;
	buffer->size = nuevo_tamanio;
	buffer->stream = nuevo_stream;


	t_paquete* paquete = empaquetar_buffer(buffer,mensaje->codigo);

	int size_serializado;
	void* serializado = serializar_paquete(paquete, &size_serializado);

	int se_envio = send(socket_cliente,serializado,size_serializado,MSG_NOSIGNAL);

	free(serializado);
	free(paquete->buffer);
	free(paquete);

	return se_envio;
}

int enviar_confirmacion(int32_t num, cod_confirmacion codigo, int socket){
	int size = sizeof(int32_t) + sizeof(cod_confirmacion);
	void* stream = malloc(size);
	int offset = 0;
	memcpy(stream + offset, &codigo, sizeof(cod_confirmacion));
	offset += sizeof(cod_confirmacion);
	memcpy(stream + offset, &num, sizeof(int32_t));

	int cuanto_se_envio = send(socket,stream,size,MSG_NOSIGNAL);

	free(stream);

	return cuanto_se_envio;
}

int32_t recibir_confirmacion(int socket, cod_confirmacion* codigo){
	cod_confirmacion codigo_recibido;
	recv(socket,&codigo_recibido,sizeof(cod_confirmacion),0);
	int32_t num;
	recv(socket,&num,sizeof(int32_t),0);
	(*codigo) = codigo_recibido;
	return num;
}

int enviar_id(int socket,int32_t id){
	int resultado = enviar_confirmacion(id,ID,socket);
	return resultado;
}

int32_t recibir_id(int socket){
	cod_confirmacion codigo;
	int32_t id =  recibir_confirmacion(socket,&codigo);
	if(codigo != ID){
		id = (-1);
	}
	return id;
}

int enviar_ACK(int socket){
	int resultado = enviar_confirmacion(1,ACK,socket);
	return resultado;
}

int recibir_ACK(int socket){
	cod_confirmacion codigo;
	recibir_confirmacion(socket,&codigo);
	int correcto = 0;
	if(codigo == ACK){
		correcto = 1;
	}
	return correcto;
}

int confirmar_suscripcion(int socket){
	int resultado = enviar_confirmacion(3,OK_SUSCRIPTO,socket);
	return resultado;
}

int recibir_confirmacion_suscripcion(int socket){
	cod_confirmacion codigo;
	recibir_confirmacion(socket,&codigo);
	int correcto = 0;
	if (codigo == OK_SUSCRIPTO) {
		correcto = 1;
	}
	return correcto;
}

t_mensaje* recibir_mensaje(int socket_cliente)
{
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));

	recv(socket_cliente, &(mensaje->codigo), sizeof(op_code),0);

	t_buffer* buffer = malloc(sizeof(t_buffer));
	recv(socket_cliente, &(buffer->size), sizeof(buffer->size),0);

	buffer->stream = malloc(buffer->size);
	recv(socket_cliente, buffer->stream, buffer->size,0);

	memcpy(&(mensaje->id), buffer->stream, sizeof(int32_t));
	buffer->stream += sizeof(int32_t);
	memcpy(&(mensaje->id_correlativo), buffer->stream, sizeof(int32_t));
	buffer->stream += sizeof(int32_t);

	buffer->size -= sizeof(int32_t) * 2;

	switch (mensaje->codigo) {
	case SUSCRIPCION:
		mensaje->mensaje = suscripcion_proceso_from_buffer(buffer);
		break;
	case NEW_POKEMON:
		mensaje->mensaje = new_pokemon_from_buffer(buffer);
		break;
	case LOCALIZED_POKEMON:
		mensaje->mensaje = localized_pokemon_from_buffer(buffer);
		break;
	case GET_POKEMON:
		mensaje->mensaje = get_pokemon_from_buffer(buffer);
		break;
	case APPEARED_POKEMON:
		mensaje->mensaje = appeared_pokemon_from_buffer(buffer);
		break;
	case CATCH_POKEMON:
		mensaje->mensaje = catch_pokemon_from_buffer(buffer);
		break;
	case CAUGHT_POKEMON:
		mensaje->mensaje = caught_pokemon_from_buffer(buffer);
		break;
	default:
		break;
	}


	return mensaje;
}

/*
 * version sin probar, controla errores en el socket que envia mensajes
t_mensaje* recibir_mensaje(int socket_cliente)
{
	t_mensaje* mensaje = malloc(sizeof(t_mensaje));

	int codigo = recv(socket_cliente, &(mensaje->codigo), sizeof(op_code),0);

	t_buffer* buffer = malloc(sizeof(t_buffer));
	int size = recv(socket_cliente, &(buffer->size), sizeof(buffer->size),0);

	buffer->stream = malloc(buffer->size);
	int buffer_recibido = recv(socket_cliente, buffer->stream, buffer->size,0);


	if(codigo > 0 && size > 0 && buffer_recibido > 0){
		memcpy(&(mensaje->id), buffer->stream, sizeof(int32_t));
		buffer->stream += sizeof(int32_t);
		memcpy(&(mensaje->id_correlativo), buffer->stream, sizeof(int32_t));
		buffer->stream += sizeof(int32_t);

		buffer->size -= sizeof(int32_t) * 2;

		switch (mensaje->codigo) {
		case SUSCRIPCION:
			mensaje->mensaje = suscripcion_proceso_from_buffer(buffer);
			break;
		case NEW_POKEMON:
			mensaje->mensaje = new_pokemon_from_buffer(buffer);
			break;
		case LOCALIZED_POKEMON:
			mensaje->mensaje = localized_pokemon_from_buffer(buffer);
			break;
		case GET_POKEMON:
			mensaje->mensaje = get_pokemon_from_buffer(buffer);
			break;
		case APPEARED_POKEMON:
			mensaje->mensaje = appeared_pokemon_from_buffer(buffer);
			break;
		case CATCH_POKEMON:
			mensaje->mensaje = catch_pokemon_from_buffer(buffer);
			break;
		case CAUGHT_POKEMON:
			mensaje->mensaje = caught_pokemon_from_buffer(buffer);
			break;
		default:
			break;
		}
	}else{
		free(mensaje);
		mensaje = NULL;
	}

	return mensaje;
}*/

char* id_to_string(int32_t id){
	char* sin_id = "| No tiene id ";
	char* con_id = string_from_format("| Su id es: %d ",id);
	char* string;
	if(id > 0){
		string = con_id;
	}else{
		string = sin_id;
	}
	return string;
}

char* id_c_to_string(int32_t id_c){
	char* sin_id = "| No tiene id correlativo.";
	char* con_id = string_from_format("| Su id correlativo es: %d.",id_c);
	char* string;
	if(id_c > 0){
		string = con_id;
	}else{
		string = sin_id;
	}
	return string;
}

char* mensaje_to_string(t_mensaje* mensaje){
	char* string_mensaje = string_new();

	char* string;

	switch(mensaje->codigo){
	case NEW_POKEMON:
		string = new_pokemon_to_string((t_new_pokemon*)mensaje->mensaje);
		break;
	case APPEARED_POKEMON:
		string = appeared_pokemon_to_string((t_appeared_pokemon*)mensaje->mensaje);
			break;
	case GET_POKEMON:
		string = get_pokemon_to_string((t_get_pokemon*)mensaje->mensaje);
			break;
	case CATCH_POKEMON:
		string = catch_pokemon_to_string((t_catch_pokemon*)mensaje->mensaje);
			break;
	case CAUGHT_POKEMON:
		string = caught_pokemon_to_string((t_caught_pokemon*)mensaje->mensaje);
			break;
	case LOCALIZED_POKEMON:
		string = localized_pokemon_to_string((t_localized_pokemon*)mensaje->mensaje);
			break;
	case SUSCRIPCION:
		string = string_from_format("Tipo = Suscripcion | Contenido = %s",suscripcion_proceso_to_string((t_suscripcion*)mensaje->mensaje));
			break;
	}

	string_append(&string_mensaje,string);

	char* id = id_to_string(mensaje->id);
	char* id_c = id_c_to_string(mensaje->id_correlativo);
	string_append(&string_mensaje, id);
	string_append(&string_mensaje, id_c);

	return string_mensaje;
}

void mensaje_mostrar(t_mensaje* mensaje){
	switch(mensaje->codigo){
		case NEW_POKEMON:
			new_pokemon_mostrar((t_new_pokemon*)mensaje->mensaje);
			break;
		case APPEARED_POKEMON:
			appeared_pokemon_mostrar((t_appeared_pokemon*)mensaje->mensaje);
				break;
		case GET_POKEMON:
			get_pokemon_mostrar((t_get_pokemon*)mensaje->mensaje);
				break;
		case CATCH_POKEMON:
			catch_pokemon_mostrar((t_catch_pokemon*)mensaje->mensaje);
				break;
		case CAUGHT_POKEMON:
			caught_pokemon_mostrar((t_caught_pokemon*)mensaje->mensaje);
				break;
		case LOCALIZED_POKEMON:
			localized_pokemon_mostrar((t_localized_pokemon*)mensaje->mensaje);
				break;
		case SUSCRIPCION:
			suscripcion_proceso_mostrar((t_suscripcion*)mensaje->mensaje);
				break;
		}
}

char* op_code_to_string(op_code codigo){
	char* get = "GET_POKEMON";
	char* new = "NEW_POKEMON";
	char* appeared = "APPEARED_POKEMON";
	char* localized = "LOCALIZED_POKEMON";
	char* catch = "CATCH_POKEMON";
	char* caught = "CAUGHT_POKEMON";
	char* op_code_string;

	switch(codigo){
	case NEW_POKEMON:
		op_code_string = new;
		break;
	case GET_POKEMON:
		op_code_string = get;
		break;
	case APPEARED_POKEMON:
		op_code_string = appeared;
		break;
	case LOCALIZED_POKEMON:
		op_code_string = localized;
		break;
	case CATCH_POKEMON:
		op_code_string = catch;
		break;
	case CAUGHT_POKEMON:
		op_code_string = caught;
		break;
	default:
		break;
	}
	return op_code_string;
}

int mensaje_size(t_mensaje* mensaje){
	int size;
	switch(mensaje->codigo){
	case NEW_POKEMON:
		size = new_pokemon_size((t_new_pokemon*)mensaje->mensaje);
		break;
	case GET_POKEMON:
		size = get_pokemon_size((t_get_pokemon*)mensaje->mensaje);
			break;
	case CATCH_POKEMON:
		size = catch_pokemon_size((t_catch_pokemon*)mensaje->mensaje);
			break;
	case CAUGHT_POKEMON:
		size = caught_pokemon_size((t_caught_pokemon*)mensaje->mensaje);
			break;
	case APPEARED_POKEMON:
		size = appeared_pokemon_size((t_appeared_pokemon*)mensaje->mensaje);
			break;
	case LOCALIZED_POKEMON:
		size = localized_pokemon_size((t_localized_pokemon*)mensaje->mensaje);
			break;
	case SUSCRIPCION:
		size = suscripcion_proceso_size((t_suscripcion*)mensaje->mensaje);
			break;
	}
	size += sizeof(int32_t)*2;
	return size;
}
