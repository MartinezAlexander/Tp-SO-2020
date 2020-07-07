#include "tall_grass.h"

/*
 * Toma dos strings, y devuelve la concatenacion de ambos
 * con el caracter '/' en el medio
 */
char* path(char* direccion, char* direccion2){
	char* path = string_duplicate(direccion);
	string_append(&path, "/");
	string_append(&path, direccion2);
	return path;
}

void crear_directorio(char* directorio){
	if(mkdir(directorio, 0777) == -1){
		puts("No se pudo crear el directorio /n");
	}
}

FILE* crear_archivo(char* directorio, char* archivo){
	char* ruta = path(directorio, archivo);
	return txt_open_for_append(ruta);
}
