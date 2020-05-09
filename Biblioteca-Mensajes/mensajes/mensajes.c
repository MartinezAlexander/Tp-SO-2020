#include "mensajes.h"
#include "appeared_pokemon.h"
#include "catch_pokemon.h"
#include "caught_pokemon.h"
#include "get_pokemon.h"
#include "localized_pokemon.h"
#include "new_pokemon.h"
#include <sys/socket.h>
#include "id_proceso.h"

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

//TODO
void enviar_mensaje(void* mensaje,op_code codigo, int socket_cliente, int32_t id, int32_t id_c)
{
	t_buffer* buffer;

	switch(codigo){
		case PROCESO:
			buffer = id_proceso_to_buffer((t_proceso*)mensaje);
			break;
		case NEW_POKEMON:
			buffer = new_pokemon_to_buffer((t_new_pokemon*) mensaje);
			break;
		case LOCALIZED_POKEMON:
			buffer = localized_pokemon_to_buffer((t_localized_pokemon*) mensaje);
			break;
		case GET_POKEMON:
			buffer = get_pokemon_to_buffer((t_get_pokemon*) mensaje);
			break;
		case APPEARED_POKEMON:
			buffer = appeared_pokemon_to_buffer((t_appeared_pokemon*) mensaje) ;
			break;
		case CATCH_POKEMON:
			buffer = catch_pokemon_to_buffer((t_catch_pokemon*) mensaje);
			break;
		case CAUGHT_POKEMON:
			buffer = caught_pokemon_to_buffer((t_caught_pokemon*) mensaje);
			break;
		default:
			break;
	}

	int nuevo_tamanio = buffer->size + 2 * sizeof(int32_t);
	void* nuevo_stream = malloc(nuevo_tamanio);
	int offset = 0;
	memcpy(nuevo_stream + offset, &id, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(nuevo_stream + offset, &id_c, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(nuevo_stream + offset, buffer->stream, buffer->size);
	offset += buffer->size;
	buffer->size = nuevo_tamanio;
	buffer->stream = nuevo_stream;


	t_paquete* paquete = empaquetar_buffer(buffer,codigo);

	int size_serializado;
	void* serializado = serializar_paquete(paquete, &size_serializado);

	send(socket_cliente,serializado,size_serializado,0);

	free(serializado);
	free(paquete->buffer);
	free(paquete);
}

void* recibir_mensaje(int socket_cliente, op_code* codigo_operacion, int32_t* id, int32_t* id_c)
{
	op_code operacion;
	recv(socket_cliente, &operacion, sizeof(operacion),0);

	t_buffer* buffer = malloc(sizeof(t_buffer));
	recv(socket_cliente, &(buffer->size), sizeof(buffer->size),0);

	buffer->stream = malloc(buffer->size);
	recv(socket_cliente, buffer->stream, buffer->size,0);

	memcpy(id,buffer->stream,sizeof(int32_t));
	buffer->stream += sizeof(int32_t);
	memcpy(id_c, buffer->stream, sizeof(int32_t));
	buffer->stream += sizeof(int32_t);

	buffer->size -= sizeof(int32_t) * 2;

	void* mensaje;
	switch(operacion){
			case PROCESO:
				 mensaje = id_proceso_from_buffer(buffer);
				break;
			case NEW_POKEMON:
				 mensaje = new_pokemon_from_buffer(buffer);
				break;
			case LOCALIZED_POKEMON:
				 mensaje = localized_pokemon_from_buffer(buffer);
				break;
			case GET_POKEMON:
				 mensaje = get_pokemon_from_buffer(buffer);
				break;
			case APPEARED_POKEMON:
				 mensaje = appeared_pokemon_from_buffer(buffer) ;
				break;
			case CATCH_POKEMON:
				 mensaje = catch_pokemon_from_buffer(buffer);
				break;
			case CAUGHT_POKEMON:
				 mensaje = caught_pokemon_from_buffer(buffer);
				break;
			default:
				break;
	}

	(*codigo_operacion) = operacion;

	return (void*)mensaje;
}
