#include "cadena.h"

op_code cadena_codigo(char* catch_pokemon){
	return MENSAJE;
}

t_buffer* cadena_to_buffer(char* mensaje){
	t_buffer* buffer = malloc( sizeof(t_buffer) );
	buffer->size = strlen(mensaje) + 1;
	buffer->stream = mensaje;
	return buffer;
}

char* cadena_from_buffer(t_buffer* buffer){
	char* mensaje = buffer->stream;
	return mensaje;
}
void cadena_mostrar(char* cadena){
	printf("Mensaje - Cadena\n");
	printf("El mensaje es %s\n",cadena);
	puts("------------");
}
void cadena_destroy(char* cadena){
	free(cadena);
}
