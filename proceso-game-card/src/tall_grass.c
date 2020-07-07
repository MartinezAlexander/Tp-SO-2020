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

void metadata_cargar(){
	char* metadata_dir = path(punto_de_montaje_tallgrass, "Metadata");
	char* metadata_path = path(metadata_dir, "Metadata.bin");

	t_config* config = config_create(metadata_path);
	if(config == NULL) puts("Error al leer Metadata/Metadata.bin");

	block_size = config_get_int_value(config, "BLOCK_SIZE");

	blocks = config_get_int_value(config, "BLOCKS");

	config_destroy(config);

	printf("[Inicializacion] Block_Size leido: %d\n", block_size);
	printf("[Inicializacion] Blocks leidos: %d\n", blocks);
}
