#include "tall_grass.h"

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

void incrementar_cantidad(t_posicion posicion){
	/*
	 * Si hay un pokemon debo eliminar la linea
	 * si no hay ninguno o hay mas incremento?
	 */
}
//TODO HACER LA FUNCION. ESTA ES MAQUETA
uint32_t* existe_posicion(t_posicion pos){
if(pos.posicionX ==0 && pos.posicionY==0){
	return 1;
}else{
	return 0;
}
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

//TODO cuando creo un archivo, ya creo un bloque aunque este vacio
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

void crear_block(int numero){
	block* bloque = malloc(sizeof(block));

	char* nombre_archivo = string_itoa(numero);
	string_append(&nombre_archivo, ".bin");
	char* ruta_directorio = path(punto_de_montaje_tallgrass, "Blocks");

	bloque->path = path(ruta_directorio, nombre_archivo);
	bloque->numero = numero;
	bloque->datos = crear_archivo(ruta_directorio, nombre_archivo);
	free(nombre_archivo);
	free(ruta_directorio);

	ocupar_bloque(numero);
	list_add(bloques, bloque);
}

void ocupar_bloque(int nuevo_bloque){
	bitarray_set_bit(estado_bloques->bitarray, nuevo_bloque);
}

int obtener_bloque_disponible(){
	for(int i = 0 ; i < blocks ; i++){
		if(!bitarray_test_bit(estado_bloques->bitarray, i)) return i;
	}
	printf("No se encontraron bloques disponibles");
	return -1;
	//TODO Si nos dicen que hacer en este caso en el enunciado resolver error.
}

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
