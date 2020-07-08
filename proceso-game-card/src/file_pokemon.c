/*
 * file_pokemon.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "file_pokemon.h"

//TODO VUELA
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

/*
 * Decrementa la cantidad de pokemones en la posicion dada para tal archivo pokemon.
 * En caso de que no queden mas luego de decrementar, elimina la posicion del archivo.
 */
void decrementar_cantidad(pokemon_file* archivo, t_posicion posicion){
	t_config* config_metadata = config_create(archivo->path);
	if(config_metadata == NULL) printf("Error al crear config de metadata\n");

	char** bloques_array = config_get_array_value(config_metadata, "BLOCKS");

	int bloque = obtener_bloque_con_posicion(bloques_array, posicion, 0);
	free(bloques_array);

	int elimino_fila = eliminar_pokemon_de_bloque(bloque, posicion);
	if(elimino_fila){
		//TODO: Si el bloque queda vacio se elimina??
		//En ese caso habria que chequear si pasa esto
		//Eliminarlo de bitmap y eliminar archivo
		//Sacarlo del array de bloques dentro del metadata

		//En caso de que no se borre, habria que actualizar el bitmap
	}

	config_destroy(config_metadata);
}


/*
 * Dado un archivo pokemon, devuelve una lista con todas
 * las posiciones en las que se encuentra dicho pokemon
 */
t_list* obtener_posiciones_pokemon(pokemon_file* archivo){
	t_list* posiciones = list_create();

	t_config* config_metadata = config_create(archivo->path);
	if(config_metadata == NULL) printf("Error al crear config de metadata\n");

	char** bloques_array = config_get_array_value(config_metadata, "BLOCKS");

	int index = 0;
	while(bloques_array[index] != NULL){
		int bloque = atoi(bloques_array[index]);

		list_add_all(posiciones, obtener_posiciones_de_bloque(bloque));
		index++;
	}

	free(bloques_array);
	config_destroy(config_metadata);

	return posiciones;
}

int existe_posicion(pokemon_file* archivo, t_posicion posicion){

	t_config* config_metadata = config_create(archivo->path);
	if(config_metadata == NULL) printf("Error al crear config de metadata\n");

	char** bloques_array = config_get_array_value(config_metadata, "BLOCKS");
	config_destroy(config_metadata);

	int bloque = obtener_bloque_con_posicion(bloques_array, posicion, 0);

	free(bloques_array);

	return bloque > -1;
}

//
//TODO VUELA (usar existe_archivo_en())
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

	char* ruta_directorio = path(path(punto_de_montaje_tallgrass, "Files"), especie);
	crear_directorio(ruta_directorio);

	archivo->path = path(ruta_directorio, "Metadata.bin");
	archivo->especie = especie;
	archivo->metadata = crear_metadata(ruta_directorio);

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
