#include "procesamiento_appeared.h"

void descartar_pokemon(t_pokemon* pokemon){
	free(pokemon);
}

void procesar_appeared(t_appeared_pokemon* appeared_pokemon){
	printf("[Appeared] Recibido: %s\n\n", appeared_pokemon->pokemon->especie);

	procesar_pokemon(appeared_pokemon->pokemon);

}

void procesar_pokemon(t_pokemon* pokemon){

	//El mutex es necesario para evitar que se planifiquen dos pokemones al mismo tiempo.
	//Tengo dos hilos con la capacidad de planificar nuevos pokemones, y ademas cada vez
	//que se me libera un entrenador me voy a fijar si hay algun pokemon en espera,
	//por lo que no debo permitir que dos planifiquen al mismo tiempo por que
	//pueden llegar a agarrar al mismo entrenador y generar inconsistencias de esa manera
	pthread_mutex_lock(&mutex_procesamiento_pokemon);
	int planifico = pokemon_dentro_de_objetivos(objetivo_global, pokemon->especie);

	if(planifico){
		pthread_mutex_lock(&mutex_diccionario_especies);
		dictionary_put(diccionario_especies_recibidas, pokemon->especie, 1);
		pthread_mutex_unlock(&mutex_diccionario_especies);

		//La idea aca es que a mi me llega un pokemon que me sirve y ya lo saco
		//de los objetivos, adelantandome, asi cuando venga otro de la misma especie
		//puedo saber cuantos necesito realmente.
		// De todas formas puedo saber si lo estoy yendo a buscar consultando
		//la referencia de objetivo actual que tiene el entrenador
		sacar_de_objetivos_globales(pokemon->especie, objetivo_global);

		entrenador_entrar_a_planificacion(pokemon);
	}else{
		//Antes de descartar me tengo que fijar si alguien ya esta
		//yendo a buscar uno de la misma especie. En ese caso, este me lo voy
		//a guardar por si ese falla.

		//Entonces me fijo si algun entrenador lo tiene asignado como
		//objetivo actual
		for(int i = 0 ; i < list_size(entrenadores) ; i++){
			t_entrenador* entrenador = list_get(entrenadores, i);
			//En caso de que este asignado, lo agrego a la cola de pendientes
			if(entrenador_tiene_objetivo(entrenador, pokemon->especie)){
				//Mutex aca porque a esta cola la voy a llamar desde otros
				//lugares y no quiero inconsistencias
				pthread_mutex_lock(&mutex_cola_espera);
				queue_push(cola_pokemones_en_espera, pokemon);
				pthread_mutex_unlock(&mutex_cola_espera);

				printf("[Planificacion] Pokemon puesto en espera, motivo: repuesto\n");
				pthread_mutex_unlock(&mutex_procesamiento_pokemon);
				return;
			}
		}

		//Finalmente si nningun entrenador lo esta yendo a buscar significa
		//que no lo necesito y lo puedo descartar
		descartar_pokemon(pokemon);
	}

	pthread_mutex_unlock(&mutex_procesamiento_pokemon);

}

void procesar_pokemon_en_espera(){
	pthread_mutex_lock(&mutex_cola_espera);

	if(!queue_is_empty(cola_pokemones_en_espera)){
		t_pokemon* pokemon = queue_pop(cola_pokemones_en_espera);
		pthread_mutex_unlock(&mutex_cola_espera);

		printf("[Planificacion] Procesamiento pokemon en espera debido a que se libero un entrenador\n");
		procesar_pokemon(pokemon);
	}else{
		pthread_mutex_unlock(&mutex_cola_espera);
	}
}
