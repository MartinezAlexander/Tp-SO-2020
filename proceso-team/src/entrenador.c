#include "entrenador.h"


void ejecutar_entrenador(t_entrenador* entrenador){

	int termine = mover_proxima_posicion(entrenador);

	if(termine){
		enviar_catch(entrenador);
	}
}

void enviar_catch(t_entrenador* entrenador){
	//Conecto con broker
	//Recibo id
	//Paso a bloqueado
	entrenador->estado = BLOCKED;
	printf("LLegue a destino, me bloqueo esperando rta \n\n");
}

//TODO averiguar que haya que usar sleep y no otra cosa.
//TODO ver otra forma de hacer este asco

//Devuelve 0 si se movio, 1 si ya esta en la posicion del objetivo
int mover_proxima_posicion(t_entrenador* entrenador){

	//Obtengo direccion
	int dirX = entrenador->objetivo_actual->posicion.posicionX - entrenador->posicion.posicionX;


	if(dirX > 0){
		entrenador->posicion.posicionX++;
		//Retardo
		sleep(retardo_cpu);
	}else if(dirX < 0){
		entrenador->posicion.posicionX--;
		//Retardo
		sleep(retardo_cpu);
	}else{

		int dirY = entrenador->objetivo_actual->posicion.posicionY - entrenador->posicion.posicionY;

		if(dirY > 0){
			entrenador->posicion.posicionY++;
			//Retardo
			sleep(retardo_cpu);
		}else if(dirY < 0){
			entrenador->posicion.posicionY--;
			//Retardo
			sleep(retardo_cpu);
		}else{
			printf("No me muevo\n");
			return 1;
		}
	}

	printf("Me movi a [%d , %d]\n", entrenador->posicion.posicionX,  entrenador->posicion.posicionY);


	return 0;
}

t_entrenador* entrenador_create(char* posicion, char* pokemones, char* objetivos){
	t_entrenador* entrenador = malloc(sizeof(t_entrenador));

	char** posiciones_separadas = string_split(posicion, "|");
	entrenador->posicion = *posicion_create( atoi(posiciones_separadas[0]) , atoi(posiciones_separadas[1]) );

	entrenador->pokemones_adquiridos = array_to_list(string_split(pokemones, "|"));
	entrenador->objetivos = array_to_list(string_split(objetivos, "|"));
	entrenador->estado = NEW;

	entrenador->objetivo_actual = NULL;

	return entrenador;
}


t_list* leer_entrenadores(t_config* config){
	char** posiciones_entrenadores = config_get_array_value(config, "POSICIONES_ENTRENADORES");
	char** pokemones_entrenadores = config_get_array_value(config, "POKEMON_ENTRENADORES");
	char** objetivos_entrenadores = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");

	int numero_posiciones = array_cantidad_de_elementos(posiciones_entrenadores);
	int numero_pok_entrenadores = array_cantidad_de_elementos(pokemones_entrenadores);
	int numero_obj_entrenadores = array_cantidad_de_elementos(objetivos_entrenadores);

	//Error si no coinciden las cantidades
	if(numero_posiciones != numero_pok_entrenadores
			|| numero_pok_entrenadores != numero_obj_entrenadores){
		printf("Error: no coindiden las cantidades de pos-pok-obj de entrenadores en config!");
	}

	t_list* entrenadores = list_create();
	for(int i = 0 ; i < numero_posiciones ; i++){
		list_add(entrenadores,entrenador_create(posiciones_entrenadores[i],
				pokemones_entrenadores[i],
				objetivos_entrenadores[i]));
	}

	return entrenadores;
}


void entrenador_mostrar(t_entrenador* entrenador){
	printf("Mostrando entrenador: \n");
	printf("Posicion: %d,%d \n", *posicion_get_X(&entrenador->posicion), *posicion_get_Y(&entrenador->posicion));
	printf("Objetivos: [");


	for(int i=0; i<list_size(entrenador->objetivos); i++){
		char* objetivos = (char*)list_get(entrenador->objetivos,i);
		printf(" %s ", objetivos);
	}

	printf("] \n");

	printf("Pokemones: [");

	for(int i=0; i<list_size(entrenador->pokemones_adquiridos); i++){
		char* pokemones = (char*)list_get(entrenador->pokemones_adquiridos,i);
		printf(" %s ", pokemones);
	}

	printf("] \n");

	printf("Estado: %d \n\n", entrenador->estado);

}

int entrenador_en_ejecucion(t_entrenador *entrenador)
{
	return(entrenador->estado == EXEC);
}
