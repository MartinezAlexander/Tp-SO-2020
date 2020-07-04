#include "deadlock.h"

void resolver_deadlock(){
	//Conseguir lista de entrenadores en deadlock y crear cola de intercambios
	t_list* listaEntrenadoresDeadlock = list_filter(entrenadores, entrenador_en_deadlock);
	//Crear lista con copias de los entrenadores (Solo con datos utiles)
	preparar_entrenadores(listaEntrenadoresDeadlock);


	//LLamar a funcion de intercambios(lista nueva con copias)
	while(!list_is_empty(listaEntrenadoresDeadlock)){
		listaEntrenadoresDeadlock = buscar_intercambios(t_list* listaEntrenadoresDeadlock);
	}
	//Mandar a mover a los entrenadores y realizar los cambios
}

void buscar_intercambios(t_list* entrenadoresDL){
	t_entrenador* primerEntrenador = list_get(entrenadoresDL, 0);
	char* primerPokemon = list_get(primerEntrenador->objetivos, 0);
	t_list* entrenadoresUtiles = list_create();

	//En entrenadores utiles agrego los entrenadores que tienen el poke que necesito
	for(int i = 1; i < list_size(entrenadoresDL); i++){
		t_entrenador* entrenador = list_get(entrenadoresDL, i);
		if(entrenador_tiene_adquirido(entrenador, primerPokemon)){
			list_add(entrenadoresUtiles, entrenador);
		}
	}
	if(list_size(entrenadoresUtiles) == 1){
		//armo y devuelvo el intercambio
	}

	//Me fijo si hay entrenadores a los que les pueda dar un pokemon util
	t_list* entrenadoresDobleIntercambio = list_create();
	for(int i = 0; i < list_size(entrenadoresUtiles); i++){
		t_entrenador* entrenador = list_get(entrenadoresUtiles, i);
		for(int y = 0; y < list_size(primerEntrenador->pokemones_adquiridos); y++){
			char* especieQueSobra = list_get(primerEntrenador->pokemones_adquiridos, y);
			if(entrenador_tiene_objetivo(entrenador, especieQueSobra)){
				list_add(entrenadoresDobleIntercambio, entrenador);
				break; //Hago break para no agregar al entrenador 2 veces a la lista de DobleInt
				//En el caso de que pueda darle 2 utiles a el.
			}
		}

	}
	if(list_size(entrenadoresDobleIntercambio) == 1){
		//armo y devuelvo el intercambio
	}else if(list_size(entrenadoresDobleIntercambio) > 1){
		//Busco al mas cercano de entrenadoresDobleIntercambio
		t_entrenador* entrenadorACambiar = obtener_entrenador_mas_cercano(entrenadoresDobleIntercambio, primerEntrenador->posicion);
		//armo y devuelvo el intercambio
	}else{
		//Busco al mas cercano pero para la primer lista entrenadoresUtiles
		t_entrenador* entrenadorACambiar = obtener_entrenador_mas_cercano(entrenadoresUtiles, primerEntrenador->posicion);
		//armo y devuelvo el intercambio
	}


}
/**
 * Prepara a los entrenadores sacando los pokemones adquiridos de la lista de objetivos
 * Asi quedan solo los pendientes en objetivos.
 * Además, sacas de adquiridos los que vas sacando de objetivos para que en adquiridos solo
 * queden los que tenes pero no necesitas
 *
 */

//TODO Modificar para nueva estructura con copia de entrenadores
void preparar_entrenadores(t_list* entrenadoresDL){
	for(int x = 0; x < list_size(entrenadoresDL); x++){
		t_entrenador* entrenador = list_get(entrenadoresDL, x);
		for(int i = 0 ; i < list_size(entrenador->pokemones_adquiridos) ; i++){
			char* pk1 = list_get(entrenador->pokemones_adquiridos, i);
			for(int j = 0; j < list_size(entrenador->objetivos); j++){
				char* pk2 = list_get(entrenador->objetivos, j);
				if(string_equals_ignore_case(pk1,pk2) == 0){
					list_remove(entrenador->objetivos, j);
					list_remove(entrenador->pokemones_adquiridos, i);
					break;
				}
			}
		}
	}
}

int entrenador_tiene_adquirido(t_entrenador* entrenador, char* especie){
	for(int i = 0; i < list_size(entrenador->pokemones_adquiridos); i++){
		if(string_equals_ignore_case(list_get(entrenador->pokemones_adquiridos, i), especie) == 0){
			return 1;
		}
	}return 0;
}

int entrenador_tiene_objetivo(t_entrenador* entrenador, char* especie){
	for(int i = 0; i < list_size(entrenador->objetivos); i++){
		if(string_equals_ignore_case(list_get(entrenador->objetivos, i), especie) == 0){
			return 1;
		}
	}return 0;
}
