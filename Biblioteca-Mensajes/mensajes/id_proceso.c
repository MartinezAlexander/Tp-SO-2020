#include "id_proceso.h"

op_code id_proceso_codigo(t_proceso* id_proceso){
	return PROCESO;
}

t_buffer* id_proceso_to_buffer(t_proceso* id_proceso){
	t_buffer* buffer = malloc( sizeof(t_buffer) );
	buffer->size = sizeof(t_proceso);
	buffer->stream = malloc(sizeof(t_proceso));
	buffer->stream = id_proceso;
	return buffer;
}

t_proceso* id_proceso_from_buffer(t_buffer* buffer){
	t_proceso* id_proceso = malloc(sizeof(t_proceso));
	id_proceso = buffer->stream;
	return id_proceso;
}
void id_proceso_mostrar(t_proceso* proceso){
	printf("Mensaje - Id Proceso\n");
	switch(*proceso){
	case TEAM:
		puts("soy un team");
		break;
	case GAMECARD:
		puts("soy un game card");
		break;
	case BROKER:
		puts("soy un broker");
		break;
	case SUSCRIPTOR:
		puts("modo suscriptor");
		break;
	}
	puts("------------");
}
void id_proceso_destroy(t_proceso* id_proceso){
	free(id_proceso);
}
