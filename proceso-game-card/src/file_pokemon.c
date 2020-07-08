/*
 * file_pokemon.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "file_pokemon.h"

//TODO no vamos a hacer asi
pokemon_file* obtener_pokemon(char* especie){
	pokemon_file* archivo = existe_pokemon(especie);
	if(archivo != NULL){
		return archivo;
	}else{
		return pokemon_file_create(especie);
	}
}


void agregar_pokemon(pokemon_file* archivo, t_posicion posicion, int cantidad){

	t_config* config_metadata = config_create(archivo->path);
	if(config_metadata == NULL) printf("Error al crear config de metadata\n");

	char** bloques_array = config_get_array_value(config_metadata, "BLOCKS");

	int bloque = obtener_bloque_con_posicion(bloques_array, posicion, cantidad);

	printf("[Insertar posicion] Bloque ya tiene posicion: %d\n", bloque);

	if(bloque > -1){
		//Algun bloque ya tiene la posicion y tiene lugar para sumarle la cantidad
		agregar_pokemon_a_bloque(bloque, posicion, cantidad);

		printf("[Insertar posicion] Modificada la cantidad de la posicion en ese bloque\n");
	}else{
		//No hay bloques que tengan la posicion con lugar para sumarle
		bloque = obtener_primer_bloque_con_espacio(bloques_array, posicion, cantidad);

		printf("[Insertar posicion] Bloque con lugar: %d\n", bloque);

		if(bloque < 0){
			bloque = obtener_bloque_disponible();

			printf("[Insertar posicion] Nuevo bloque creado para posicion: %d\n", bloque);

			//TODO: catchear caso extremo de que devuelva -1 (no hay bloques libres)
			crear_block(bloque);

			//Actualizo los bloques en el metadata
			char* bloques_string = config_get_string_value(config_metadata, "BLOCKS");
			bloques_string = string_substring(bloques_string, 0, string_length(bloques_string) - 1);
			char* bloques_actualizados = string_from_format("%s,%d]", bloques_string, bloque);

			config_set_value(config_metadata, "BLOCKS", bloques_actualizados);
			config_save(config_metadata);
			free(bloques_actualizados);
			free(bloques_string);
		}

		agregar_nuevo_pokemon_a_bloque(bloque, posicion, cantidad);

		printf("[Insertar posicion] Modificada la cantidad de la posicion en ese bloque\n");
	}

	config_destroy(config_metadata);
}


void decrementar_cantidad(pokemon_file* archivo, t_posicion posicion){
// Si no hay ninguno no deberia estar la linea
	// Si hay un pokemon debo eliminar la linea
	/*
	t_config* bloque = obtener_bloque_por_indice(2);
		int x = config_get_int_value(bloque,"4-4");
		printf("el valor es %d",x);

		if(x==1){
			config_remove_key(bloque,"4-4");
			config_save(bloque);
			printf("el valor es %d",x);
		}
		*/
	 // Si hay mas decremento en uno la cantidad


}

//TODO Me dan un pokemon y tengo que hacer una lista con las posiciones que existen
t_list* obtener_posiciones(char* pokemon){

	return NULL;
}

//TODO HACER LA FUNCION.
int existe_posicion(pokemon_file* archivo, t_posicion pos){

	return 0;
}

//TODO no vamos a hacer asi. Hacer de nuevo
pokemon_file* existe_pokemon(char* especie){
	for(int i = 0 ; i < list_size(pokemons) ; i++){
		pokemon_file* p = list_get(pokemons, i);
		if(p->especie == especie)
			return p;
	}
	return NULL;
}

//TODO: sincronizacion para abrir y cerrar archivos
void abrir_archivo(pokemon_file* archivo){

	//Uso config para leer el archivo metadata
	t_config* config_metadata = config_create(archivo->path);
	if(config_metadata == NULL) printf("Error al crear config de metadata\n");

	char* valor_open = config_get_string_value(config_metadata, "OPEN");

	//En caso que ya este abiero espero x segundos
	while(string_equals_ignore_case(valor_open, "Y")){
		sleep(tiempo_reintento_operacion);
		valor_open = config_get_string_value(config_metadata, "OPEN");
	}

	//Cambiamos el valor de OPEN a Y
	config_set_value(config_metadata, "OPEN", "Y");
	config_save(config_metadata);
	config_destroy(config_metadata);

}

void cerrar_archivo(pokemon_file* archivo){
	//Uso config para leer el archivo metadata
	t_config* config_metadata = config_create(archivo->path);
	if(config_metadata == NULL) printf("Error al crear config de metadata\n");
	config_set_value(config_metadata, "OPEN", "N");
	config_save(config_metadata);
	config_destroy(config_metadata);
}

pokemon_file* pokemon_file_create(char* especie){
	pokemon_file* archivo = malloc(sizeof(pokemon_file));

	/*char* ruta_directorio = path(path(punto_de_montaje_tallgrass, "Files"), especie);
	crear_directorio(ruta_directorio);*/

	char* directorio_files = obtener_directorio_files();
	crear_directorio(directorio_files,especie);

	archivo->path = path(directorio_files, "Metadata.bin");
	archivo->especie = especie;
	archivo->metadata = crear_metadata(directorio_files);

	//list_add(pokemons, archivo);
	return archivo;
}

FILE* crear_metadata(char* ruta){
	int bloque_numero = obtener_bloque_disponible();
	crear_block(bloque_numero);

	FILE* metadata = crear_archivo(ruta, "Metadata.bin");
	txt_write_in_file(metadata, "DIRECTORY=N\n");
	txt_write_in_file(metadata, "SIZE=0\n");
	txt_write_in_file(metadata, string_from_format("BLOCKS=[%s]\n", string_itoa(bloque_numero)));
	txt_write_in_file(metadata, "OPEN=N\n");
	txt_close_file(metadata);

	return metadata;
}
