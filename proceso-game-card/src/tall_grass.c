#include "tall_grass.h"

/*
 * Toma dos strings, y devuelve la concatenacion de ambos
 * con el caracter '/' en el medio
 */
//TODO Locks held: none [PID: 8835]
char* path(char* direccion, char* direccion2) {
	char* path = string_duplicate(direccion);
	string_append(&path, "/");
	//TODO Address 0x423ffb0 is 32 bytes inside a block of size 47 alloc'd [PID: 8835]
	string_append(&path, direccion2);
	//(aprox 40 en promedio) 56 bytes in 1 blocks are definitely lost (x casi infinitas veces)
	return path;
}

void crear_directorio(char* en_donde_crear, char* nombre_directorio) {
	char* ruta = path(en_donde_crear, nombre_directorio);
	if (mkdir(ruta, 0777) == -1) {
		puts("No se pudo crear el directorio /n");
	}
	free(ruta);
}

FILE* crear_archivo(char* directorio, char* archivo) {
	char* ruta = path(directorio, archivo);
	FILE* file = txt_open_for_append(ruta);
	free(ruta);
	return file;
}

void metadata_cargar() {
	char* metadata_dir = path(punto_de_montaje_tallgrass, "Metadata");
	char* metadata_path = path(metadata_dir, "Metadata.bin");
	free(metadata_dir);

	t_config* config = config_create(metadata_path);
	free(metadata_path);
	if (config == NULL)
		puts("Error al leer Metadata/Metadata.bin");

	block_size = config_get_int_value(config, "BLOCK_SIZE");

	blocks = config_get_int_value(config, "BLOCKS");

	config_destroy(config);

	printf("[Inicializacion] Block_Size leido: %d\n", block_size);
	printf("[Inicializacion] Blocks leidos: %d\n", blocks);
}

t_list* listar_directorio(char* path) {
	DIR* directorio;
	struct dirent* elemento;
	t_list* lista_directorios = list_create();
	directorio = opendir(path);

	if (directorio != NULL) {
		elemento = readdir(directorio);
		while (elemento) {
			char* nombre = string_new();
			string_append(&nombre, elemento->d_name);
			list_add(lista_directorios, nombre);
			//puts(nombre);
			elemento = readdir(directorio);
		}
	}
	closedir(directorio);
	return lista_directorios;
}

int existe_archivo_en(char* nombre_archivo, char* path) {
	int existe = 0;
	pthread_mutex_lock(&mutex_listar_directorios);
	t_list* lista_directorios = listar_directorio(path);
	pthread_mutex_unlock(&mutex_listar_directorios);

	for (int i = 0; i < list_size(lista_directorios); i++) {
		char* nombre = list_get(lista_directorios, i);
		if (string_equals_ignore_case(nombre_archivo, nombre)) {
			existe = 1;
		}
	}
	list_destroy_and_destroy_elements(lista_directorios, free);
	return existe;
}

char* obtener_directorio_blocks() {
	char* path_blocks = NULL;

	if (existe_archivo_en("Blocks", punto_de_montaje_tallgrass)) {
		path_blocks = path(punto_de_montaje_tallgrass, "Blocks");
	}

	return path_blocks;
}

char* obtener_directorio_files() {
	char* path_files = path(punto_de_montaje_tallgrass, "Files");

	if (!existe_archivo_en("Files", punto_de_montaje_tallgrass)) {
		crear_directorio(punto_de_montaje_tallgrass, "Files");
	}

	return path_files;
}

char* obtener_directorio_metadata() {
	char* path_metadata = path(punto_de_montaje_tallgrass, "Metadata");

	if (!existe_archivo_en("Metadata", punto_de_montaje_tallgrass)) {
		crear_directorio(punto_de_montaje_tallgrass, "Metadata");

		//DUDA, ESTO ES VIEJO?

		//TODO Crear archivo metadata propio y cargarlo con valores default?

		//TODO (luego de responder duda) Crear archivo metadata propio y cargarlo con valores default?

		//crear_archivo(punto_de_montaje_tallgrass,"Metadata.bin");
		//cargar archivo metadata
	}

	return path_metadata;
}

char* obtener_directorio_bitmap() {
	char* path_metadata = obtener_directorio_metadata();
	char* path_bitmap = path(path_metadata, "Bitmap.bin");

	if (!existe_archivo_en("Bitmap.bin", path_metadata)) {
		FILE* fd = crear_archivo(path_metadata, "Bitmap.bin");
		fclose(fd);
	}

	return path_bitmap;
}