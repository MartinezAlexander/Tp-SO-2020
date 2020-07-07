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
	//Esto no nos quedo claro como se hace
	//Nosotros tenemos en el metadata, todos los bloques en los que hay datos
	//Si algun bloque ya tiene mi posicion, me imagino que la sumamos ahi
	//Lo que seria, recorrer cada bloque y ver si tiene mi posicion
	//Ahora, si no esta en ningun bloque que hago:
	//Creo uno nuevo y lo meto ahi??
	//Me fijo en que bloque hay lugar y lo meto ahi??
	//Como se cuanto lugar ocupa, literalmente el peso los caracteres??
}

void incrementar_cantidad(pokemon_file* archivo, t_posicion posicion){
	/*
	 * Si hay un pokemon debo eliminar la linea
	 * si no hay ninguno o hay mas incremento?
	 */
}

//TODO Me dan un pokemon y tengo que hacer una lista con las posiciones que existen
t_list* obtener_posiciones(char* pokemon){
	t_list* posiciones;
	return posiciones;
}
//TODO HACER LA FUNCION.
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

//TODO: sacar el valor open de cada archivo y leer del archivo ese valor
//Ademas, no tenemos que dejar el archivo abierto al final, sino que lo abro y lo cierro
//cuando necesite (me refiero a hacer fopen() )
void abrir_archivo(pokemon_file* archivo){
	//En caso que ya este abiero espero x segundos
	while(archivo->open == 1){
		sleep(tiempo_reintento_operacion);
	}
	//Finalmente, abro el archivo
	archivo->open = 1;
	//Bueno, aca tenemos dos alternativas:
	//Usar seek y todas esas funciones primitivas de archivos
	//para sobreescribir el OPEN.
	//O usar de las commons el config. (medio turbio) -PREGUNTAR SI ES LEGAL!

	//Cambiamos el valor de OPEN a Y
	t_config* config_metadata = config_create(archivo->path);
	if(config_metadata == NULL) printf("Error al crear config de metadata\n");
	config_set_value(config_metadata, "OPEN", "Y");
	config_save(config_metadata);
	config_destroy(config_metadata);
	//Dejamos el archivo abierto
	txt_open_for_append(archivo->path);								//ACA   <=
}

void cerrar_archivo(pokemon_file* archivo){
	txt_close_file(archivo->metadata);
	archivo->open = 0;
}

pokemon_file* pokemon_file_create(char* especie){
	pokemon_file* archivo = malloc(sizeof(pokemon_file));

	char* ruta_directorio = path(path(punto_de_montaje_tallgrass, "Files"), especie);
	crear_directorio(ruta_directorio);

	archivo->path = path(ruta_directorio, "Metadata.bin");
	archivo->especie = especie;
	archivo->metadata = crear_metadata(ruta_directorio);
	archivo->open = 0;

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
