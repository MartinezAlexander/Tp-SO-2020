#include "deadlock.h"

void resolver_deadlock(){

	//Pasar entrenadores a ent_intercambio
	t_list* entrenadores_copia = list_create();

	for(int i = 0 ; i < list_size(entrenadores) ; i++){
		t_entrenador* entrenador = list_get(entrenadores, i);
		t_entrenador_copia* e = entrenador_copia_create(entrenador);
		list_add(entrenadores_copia, e);
	}

	preparar_entrenadores(entrenadores_copia);

	cola_intercambios_deadlock = queue_create();
	while(!list_is_empty(entrenadores_copia)){
		t_intercambio* intercambio = primer_intercambio(entrenadores_copia);

		printf("[Deadlock] Nuevo intercambio programado entre entrenadores: %d y %d, para especies %s por %s\n",
				intercambio->entrenador->identificador, intercambio->entrenadorObjetivo->identificador,
				intercambio->pokemonADar, intercambio->pokemonARecibir);

		queue_push(cola_intercambios_deadlock, intercambio);
	}

	encolar_proximo_intercambio(1);
}

void encolar_proximo_intercambio(int primer_intercambio){
	if(queue_is_empty(cola_intercambios_deadlock)){
		//TODO [DL] Avisar de alguna manera que termino? O ya de por si por los estados termina?
		return;
	}

	t_intercambio* intercambio = queue_pop(cola_intercambios_deadlock);
	t_entrenador* entrenador = intercambio->entrenador;
	entrenador->intercambio_actual = intercambio;
	encolar(entrenador);

	if(primer_intercambio) planificar();
}

t_intercambio* primer_intercambio(t_list* entrenadores_deadlock){
	t_entrenador_copia* primer_entrenador = list_get(entrenadores_deadlock, 0);

	char* pokemonARecibir = list_get(primer_entrenador->objetivos, 0);

	t_list* entrenadores_posibles = filtrar_entrenadores_con_pokemon(1, entrenadores_deadlock, pokemonARecibir);

	t_list* posibles_intercambios = obtener_posibles_intercambios(entrenadores_posibles, primer_entrenador, pokemonARecibir);

	t_list* intercambios = filtrar_dobles_intercambios(posibles_intercambios);

	t_intercambio_copia* mejor_intercambio = obtener_intercambio_mas_cercano(intercambios);

	//Antes de terminar tengo que realizar el cambio 'simbolicamente'
	//asi tengo esto en cuenta para detectar los proximos intercambios
	realizar_intercambio_simbolico(mejor_intercambio);

	/*
	 * Preparo los entrenadores afectados, asi puedo ver si siguen teniendo objetivos
	 * sin cumplir y los puedo sacar de la lista en caso contrario
	 */
	preparar_entrenadores(entrenadores_deadlock);

	return convertir_a_intercambio_real(mejor_intercambio);
}

//TODO: Ver si la clase intercambio_copia me sirve realmente

/*
 * Devuelve el intercambio cuyos movimientos necesarios para llevarse a cabo
 * sea la menor cantidad entre todos los del listado
 */
t_intercambio_copia* obtener_intercambio_mas_cercano(t_list* intercambios){
	int cantidad_intercambios = list_size(intercambios);

	t_intercambio_copia* primer_intercambio = list_get(intercambios, 0);

	//De nuevo, si hay uno solo devuelvo ese
	if(cantidad_intercambios == 1) return primer_intercambio;

	int index_mas_cercano = 0;
	int distancia_mas_corta = movimientos_entre_posiciones(primer_intercambio->entrenador->posicion, primer_intercambio->entrenadorObjetivo->posicion);

	for(int i = 1 ; i < cantidad_intercambios ; i++){
		t_intercambio_copia* intercambio = list_get(intercambios, i);

		int distancia = movimientos_entre_posiciones(intercambio->entrenador->posicion, intercambio->entrenadorObjetivo->posicion);

		if(distancia < distancia_mas_corta){
			index_mas_cercano = i;
			distancia_mas_corta = distancia;
		}
	}

	return list_get(intercambios, index_mas_cercano);
}

/*
 * Una de las prioridades que damos para encontrar el mejor intercambio es
 * chequear si el intercambio es doble; osea, si yo como entrenador que voy
 * a recibir el que necesito, puedo darle al otro entrenador uno que el necesite.
 *
 * Le damos mas prioridad al doble intercambio que a la distancia entre ellos, ya
 * que si detectamos los de doble antes, nos ahorramos mas intercambios, y hasta podriamos
 * tener entrenadores que terminen de ejecutar antes gracias a esto.
 */
t_list* filtrar_dobles_intercambios(t_list* intercambios){
	int cantidad_intercambios = list_size(intercambios);

	//Si veo que mi lista solo tiene un item no tiene sentido tratar de filtrar
	//asi que devuelvo la lista que recibi asi sigo con esa
	if(cantidad_intercambios == 1) return intercambios;

	t_list* dobles_intercambios = list_create();

	for(int i = 0 ; i < cantidad_intercambios ; i++){
		t_intercambio_copia* intercambio = list_get(intercambios, i);

		if(hay_doble_intercambio(intercambio)) list_add(dobles_intercambios, intercambio);
	}

	//En caso de que no encontre ningun intercambio doble, me voy a quedar con mi
	//lista de intercambios simples
	return list_size(dobles_intercambios) > 0 ? dobles_intercambios : intercambios;
}

int hay_doble_intercambio(t_intercambio_copia* intercambio){
	/*
	 * Yo ya se que el otro entrenador tiene como adquirido el poke que yo
	 * necesito, asi que lo unico que me fijo para saber si hay doble intercambio
	 * es ver que el que yo le voy a dar este en sus objetivos
	 */
	return entrenador_tiene_en_objetivo(intercambio->entrenadorObjetivo, intercambio->pokemonADar);
}

/*
 * Dada una lista de entrenadores que poseen el pokemon a recibir, arma todos los posibles
 * intercambios con el entrenador principal, osea, para cada entrenador candidato
 * y dandole cada posible pokemon que le sobre al entrenador principal
 */
t_list* obtener_posibles_intercambios(t_list* candidatos, t_entrenador_copia* entrenador_intercambio, char* especieARecibir){

	t_list* intercambios = list_create();

	for(int i = 0 ; i < list_size(candidatos) ; i++){
		t_entrenador_copia* entrenador_candidato = list_get(candidatos, i);

		//Volvemos hacer algo que haciamos en el envio de GET:
		//Puede pasar que mi entrenador tenga varios adquiridos de la misma especie,
		//en ese caso no quiero armar intercambio dos veces, entonces guardo
		//referencia de la ultima especie a dar, y comparo que sean distintas
		//para armar el intercambio
		//Ya sabemos que las listas estan ordenadas. Por eso funciona esto
		char* especieAnterior = "";
		for(int j = 0 ; j < list_size(entrenador_intercambio->adquiridos) ; j++){
			char* especieADar = list_get(entrenador_intercambio->adquiridos, j);

			if(!string_equals_ignore_case(especieAnterior, especieADar)){
				especieAnterior = especieADar;

				t_intercambio_copia* nuevo_intercambio = intercambio_copia_create(entrenador_intercambio, entrenador_candidato, especieADar, especieARecibir);
				list_add(intercambios, nuevo_intercambio);
			}
		}
	}

	return intercambios;
}

t_list* filtrar_entrenadores_con_pokemon(int desde_index, t_list* entrenadores_deadlock, char* especie){
	t_list* filtrados = list_create();

	for(int i = desde_index ; i < list_size(entrenadores_deadlock) ; i++){
		t_entrenador_copia* entrenador = list_get(entrenadores_deadlock, i);

		if(entrenador_tiene_adquirido(entrenador, especie)) list_add(filtrados, entrenador);
	}

	return filtrados;
}




/**
 * Prepara a los entrenadores sacando los pokemones adquiridos de la lista de objetivos
 * Asi quedan solo los pendientes en objetivos.
 * Además, sacas de adquiridos los que vas sacando de objetivos para que en adquiridos solo
 * queden los que tenes pero no necesitas
 *
 */

void preparar_entrenadores(t_list* entrenadoresDL){

	/*
	 * Nota: por si nos pasa de nuevo.
	 * Estamos queriendo eliminar de un for andando, entonces para
	 * evitar que al eliminar se me corra el indice y no lo itere, le hago i--
	 */
	for(int i = 0; i < list_size(entrenadoresDL); i++){
		t_entrenador_copia* entrenador = list_get(entrenadoresDL, i);
		int quedan_objetivos = preparar_entrenador(entrenador);

		if(!quedan_objetivos){
			list_remove(entrenadoresDL, i);
			i--;
		}
	}
}

/*
 * Se asegura que no haya matcheos entre los objetivos y los pokemones
 * adquiridos de un entrenador. Nos es de utilidad para poder manejarnos
 * solo con los pokemones que necesita y no tener que ir filtrando los
 * objetivos que ya cumplio.
 *
 * Devuelve 1 en caso de que siga teniendo objetivos
 *	0 en caso de que ya cumplio sus objetivos (en realidad va a cumplirlos)
 */
int preparar_entrenador(t_entrenador_copia* entrenador){


	for(int i = 0 ; i < list_size(entrenador->adquiridos) ; i++){
		char* pk1 = list_get(entrenador->adquiridos, i);

		for(int j = 0; j < list_size(entrenador->objetivos); j++){
			char* pk2 = list_get(entrenador->objetivos, j);

			if(string_equals_ignore_case(pk1,pk2)){
				list_remove(entrenador->objetivos, j);
				list_remove(entrenador->adquiridos, i);
				i--;
				break;
			}
		}
	}

	//Necesitamos que esten ordenadas para un toque mas adelante,
	//OJO, se ordena de Mayor a Menor por algun motivo
	list_sort(entrenador->adquiridos, strcmp);
	list_sort(entrenador->objetivos, strcmp);

	return !list_is_empty(entrenador->objetivos);
}

/*
 * Devuelve 1 si el entrenador posee, dentro de sus pokemones de sobra,
 * la especie dada
 */
int entrenador_tiene_adquirido(t_entrenador_copia* entrenador, char* especie){
	for(int i = 0; i < list_size(entrenador->adquiridos); i++){
		char* poke = list_get(entrenador->adquiridos, i);

		if(string_equals_ignore_case(poke, especie)){
			return 1;
		}
	}return 0;
}

/*
 * Devuelve 1 si el entrenador necesita dicha especie para cumplir su objetivo
 */
int entrenador_tiene_en_objetivo(t_entrenador_copia* entrenador, char* especie){
	for(int i = 0; i < list_size(entrenador->objetivos); i++){
		if(string_equals_ignore_case(list_get(entrenador->objetivos, i), especie)){
			return 1;
		}
	}return 0;
}


t_intercambio* convertir_a_intercambio_real(t_intercambio_copia* intercambio_copia){
	return intercambio_create(intercambio_copia->entrenador->entrenador,
			intercambio_copia->entrenadorObjetivo->entrenador,
			intercambio_copia->pokemonADar, intercambio_copia->pokemonARecibir);
}

void realizar_intercambio_simbolico(t_intercambio_copia* intercambio){

	cambiar_pokemon(intercambio->entrenador->adquiridos, intercambio->pokemonADar, intercambio->pokemonARecibir);
	cambiar_pokemon(intercambio->entrenadorObjetivo->adquiridos, intercambio->pokemonARecibir, intercambio->pokemonADar);

	intercambio->entrenador->posicion = intercambio->entrenadorObjetivo->posicion;
}

void cambiar_pokemon(t_list* listado_pokemon, char* especieASacar, char* especieAMeter){

	for(int i = 0 ; i < list_size(listado_pokemon) ; i++){
		char* pok = list_get(listado_pokemon, i);

		if(string_equals_ignore_case(pok, especieASacar)){
			list_remove(listado_pokemon, i);
			break;
		}
	}

	list_add(listado_pokemon, especieAMeter);
}


//------------------------CREATES-----------------------------

t_intercambio* intercambio_create(t_entrenador* entrenador, t_entrenador* entrenadorObjetivo, char* pokemonADar, char* pokemonARecibir){
	t_intercambio* intercambio = malloc(sizeof(t_intercambio));
	intercambio->entrenador = entrenador;
	intercambio->entrenadorObjetivo = entrenadorObjetivo;
	intercambio->pokemonADar = pokemonADar;
	intercambio->pokemonARecibir = pokemonARecibir;

	return intercambio;
}

t_intercambio_copia* intercambio_copia_create(t_entrenador_copia* entrenador, t_entrenador_copia* entrenadorObjetivo, char* pokemonADar, char* pokemonARecibir){
	t_intercambio_copia* intercambio = malloc(sizeof(t_intercambio_copia));
	intercambio->entrenador = entrenador;
	intercambio->entrenadorObjetivo = entrenadorObjetivo;
	intercambio->pokemonADar = pokemonADar;
	intercambio->pokemonARecibir = pokemonARecibir;

	return intercambio;
}

t_entrenador_copia* entrenador_copia_create(t_entrenador* entrenador){
	t_entrenador_copia* entrenador_intercambio = malloc(sizeof(t_entrenador_copia));
	entrenador_intercambio->entrenador = entrenador;
	entrenador_intercambio->objetivos = entrenador->objetivos;
	entrenador_intercambio->adquiridos = entrenador->pokemones_adquiridos;
	entrenador_intercambio->posicion = entrenador->posicion;
	return entrenador_intercambio;
}
