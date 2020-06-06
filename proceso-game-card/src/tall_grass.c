#include "tall_grass.h"

FILE* buscar_pokemon(char* especie){
	//Entro a FILES, busco si esta el directorio de especie, si no esta, lo creo.
	char* path = path(punto_de_montaje_tallgrass, "Files");
	path = path(path, especie);
	if(existe_directorio(path)){
		return abrir_directorio(path);
	}else{
		crear_directorio(path);
		FILE* nuevo_metadata = crear_archivo(path, "Metadata.bin");
		return crear_metadata(nuevo_metadata);
	}
}

int existe_directorio(char* path){
	return 0;
}

FILE* abrir_directorio(char* path){
	return NULL;
}

FILE* crear_metadata(FILE* metadata){
	//Abrir archivo?
	//Directory = N
	//Size = Bloques x tam bloques
	//Buscar bloque disponible (?)
	//Blocks = [bloque]
	//Open = N
	//Cerrar archivo
	return metadata;
}

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
	char* path = path(directorio, archivo);
	FILE* file = fopen(path  , "w");
	if(file == NULL){
		puts("No se pudo crear el archivo");
	}
	fclose(file);
	return file;
}

void abrir_archivo(FILE* archivo){
	//Se fija si lo puede abrir, si no, reintenta cada X seg (Archivo config)
}
