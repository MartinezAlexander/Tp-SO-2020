#include "suscripcion.h"
#include <commons/string.h>

t_suscripcion* suscripcion_proceso_create(t_proceso id_proceso, int32_t pid, op_code cola){
	t_suscripcion* nueva_sub = malloc( sizeof(t_suscripcion) );
	nueva_sub->id = id_proceso;
	nueva_sub->pid = pid;
	nueva_sub->cola_suscripcion = cola;
	return nueva_sub;
}

op_code suscripcion_proceso_codigo(t_suscripcion* id_proceso){
	return SUSCRIPCION;
}

t_buffer* suscripcion_proceso_to_buffer(t_suscripcion* suscripcion){
	t_buffer* buffer = malloc( sizeof(t_suscripcion) );
	buffer->size = sizeof(int32_t) + sizeof(t_proceso) + sizeof(op_code);
	buffer->stream = malloc(buffer->size);

	int offset = 0;

	memcpy(buffer->stream + offset, &(suscripcion->id), sizeof(t_proceso));
	offset += sizeof(t_proceso);
	memcpy(buffer->stream + offset, &(suscripcion->pid), sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(buffer->stream + offset, &(suscripcion->cola_suscripcion), sizeof(op_code));
	offset += sizeof(op_code);

	return buffer;
}

t_suscripcion* suscripcion_proceso_from_buffer(t_buffer* buffer){
	t_suscripcion* suscripcion = malloc(sizeof(t_suscripcion));
	memcpy(&(suscripcion->id),buffer->stream, sizeof(t_proceso));
	buffer->stream += sizeof(t_proceso);
	memcpy(&(suscripcion->pid), buffer->stream, sizeof(int32_t));
	buffer->stream += sizeof(int32_t);
	memcpy(&(suscripcion->cola_suscripcion), buffer->stream, sizeof(op_code));
	buffer->stream += sizeof(op_code);
	return suscripcion;
}

void que_cola_soy(op_code codigo){
	switch(codigo){
		case NEW_POKEMON:
			printf("Cola a suscribirme: NEW_POKEMON\n");
			break;
		case APPEARED_POKEMON:
			printf("Cola a suscribirme: APPEARED_POKEMON\n");
			break;
		case GET_POKEMON:
			printf("Cola a suscribirme: GET_POKEMON\n");
			break;
		case CATCH_POKEMON:
			printf("Cola a suscribirme: CATCH_POKEMON\n");
			break;
		case CAUGHT_POKEMON:
			printf("Cola a suscribirme: CAUGHT_POKEMON\n");
			break;
		case LOCALIZED_POKEMON:
			printf("Cola a suscribirme: LOCALIZED_POKEMON\n");
			break;
		case SUSCRIPCION:
			break;
		}
}

void que_proceso_soy(t_proceso proceso){
	switch(proceso){
	case TEAM:
		printf("Proceso: TEAM\n");
		break;
	case GAMECARD:
		printf("Proceso: GAMECARD\n");
		break;
	case BROKER:
		printf("Proceso: BROKER\n");
		break;
	case GAMEBOY:
		printf("Proceso: GAMEBOY\n");
		break;
	case SUSCRIPTOR:
		break;
	}
}

void suscripcion_proceso_mostrar(t_suscripcion* suscripcion){
	printf("Mensaje - Suscripcion Proceso\n");
	que_proceso_soy(suscripcion->id);
	printf("Mi pid es: %d\n",suscripcion->pid);
	que_cola_soy(suscripcion->cola_suscripcion);
	puts("------------");
}

char* proceso_to_string(t_proceso proceso){
	char* team = "TEAM";
	char* gamecard = "GAMECARD";
	char* gameboy = "GAMEBOY";
	char* suscriptor = "SUSCRIPTOR";
	char* broker = "BROKER";
	char* nombre_proceso;
	switch(proceso){
		case TEAM:
			nombre_proceso = team;
			break;
		case GAMECARD:
			nombre_proceso = gamecard;
			break;
		case GAMEBOY:
			nombre_proceso = gameboy;
			break;
		case SUSCRIPTOR:
			nombre_proceso = suscriptor;
			break;
		case BROKER:
			nombre_proceso = broker;
			break;
	}
	return nombre_proceso;
}

char* cola_to_string(op_code cola){
	char* cola_new = "cola: NEW_POKEMON";
	char* cola_appeared = "cola: APPEARED_POKEMON";
	char* cola_get = "cola: GET_POKEMON";
	char* cola_localized = "cola: LOCALIZED_POKEMON";
	char* cola_catch = "cola: CATCH_POKEMON";
	char* cola_caught = "cola: CAUGHT_POKEMON";
	char* nombre_cola;

	switch(cola){
		case NEW_POKEMON:
			nombre_cola = cola_new;
			break;
		case APPEARED_POKEMON:
			nombre_cola = cola_appeared;
			break;
		case GET_POKEMON:
			nombre_cola = cola_get;
			break;
		case LOCALIZED_POKEMON:
			nombre_cola = cola_localized;
			break;
		case CATCH_POKEMON:
			nombre_cola = cola_catch;
			break;
		case CAUGHT_POKEMON:
			nombre_cola = cola_caught;
			break;
		default:
			break;
	}
	return nombre_cola;
}

char* suscripcion_proceso_to_string(t_suscripcion* suscripcion){
	char* nombre_proceso = proceso_to_string(suscripcion->id);
	char* nombre_cola = cola_to_string(suscripcion->cola_suscripcion);
	return string_from_format("Mensaje - Suscripcion Proceso\nProceso: %s\nMi pid es %d\nCola a suscribirme: %s\n------------",nombre_proceso,suscripcion->pid, nombre_cola);
}

void suscripcion_proceso_destroy(t_suscripcion* suscripcion){
	free(suscripcion);
}