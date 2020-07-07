/*
 * file_pokemon.c
 *
 *  Created on: 7 jul. 2020
 *      Author: utnso
 */

#include "file_pokemon.h"

pokemon_file* obtener_pokemon(char* especie){
	pokemon_file* archivo = existe_pokemon(especie);
	if(archivo != NULL){
		return archivo;
	}else{
		return pokemon_file_create(especie);
	}
}


void agregar_pokemon(pokemon_file* archivo, t_posicion posicion, int cantidad){
	char* posicion_string = string_from_format("%d-%d", posicion.posicionX, posicion.posicionY);

	t_config* config_metadata = config_create(archivo->path);
	if(config_metadata == NULL) printf("Error al crear config de metadata\n");

	char** bloques = config_get_array_value(config_metadata, "BLOCKS");

	int index = 0;
	while(bloques[index] != NULL){

		//FILE* bloque = obtener_bloque_por_indice(atoi(bloques[index]));
		char* bloque_archivo = string_from_format("%s.bin", bloques[index]);
		char* path_bloque = path(path(punto_de_montaje_tallgrass, "Blocks"), bloque_archivo);

		t_config* config_bloque = config_create(path_bloque);
		if(config_bloque == NULL) printf("Error al crear config de bloque\n");

		if(config_has_property(config_bloque, posicion_string)){
			int cantidad_en_posicion = config_get_int_value(config_bloque, posicion_string);
			//TODO: chequear que al agregar no se pase el tamaño?
			config_set_value(config_bloque, posicion_string, string_itoa(cantidad_en_posicion + cantidad));
			config_save(config_bloque);
			config_destroy(config_bloque);

			break;
		}

		//int obtener_tamanio_ocupado_por_bloque(numero)

		//TODO: DUDA -> los bloques se llenan a medida que hay lugar o en cualquiera?
		//osea si mi archivo usa 4 bloques y yo quiero meter algo nuevo, voy
		//a tener que ir al ultimo bloque (o a lo sumo crear uno nuevo) o puede
		//que haya lugar en alguno de los bloques del medio???

		config_save(config_bloque);
		config_destroy(config_bloque);

		index++;
	}

	//busco la posicion. Si encuentro sumo cantidad
	//actualizo tamaños
	//Si no hay posicion creo posicion en el primer bloque que haya lugar? o en el ultimo?
	//Si no hay lugar en ninguno, creo un bloque nuevo

	config_save(config_metadata);
	config_destroy(config_metadata);
}

void incrementar_cantidad(pokemon_file* archivo, t_posicion posicion){
	/*
	 * Si hay un pokemon debo eliminar la linea
	 * si no hay ninguno o hay mas incremento?
	 */
}

//TODO HACER LA FUNCION. ESTA ES MAQUETA
int existe_posicion(pokemon_file* archivo, t_posicion pos){
return 0;
}

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

	list_add(pokemons, archivo);
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
