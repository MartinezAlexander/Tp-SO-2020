#include "deadlock.h"


void resolver_deadlock(){
	loggear_inicio_deteccion_deadlock();

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

	list_destroy(entrenadores_copia);

	deadlocks_detectados = queue_size(cola_intercambios_deadlock);
	loggear_resultado_deteccion_deadlock(deadlocks_detectados);

	encolar_proximo_intercambio(1);
}

void encolar_proximo_intercambio(int primer_intercambio){
	if(queue_is_empty(cola_intercambios_deadlock)){
		printf("[Deadlock] Se ha terminado la resolucion de deadlock exitosamente\n");
		return;
	}

	t_intercambio* intercambio = queue_pop(cola_intercambios_deadlock);
	t_entrenador* entrenador = intercambio->entrenador;
	entrenador->intercambio_actual = intercambio;
	encolar(entrenador);

	printf("[Deadlock] Encolando a entrenador %d para resolver intercambio de deadlock\n", entrenador->identificador);

	if(primer_intercambio) planificar();
}

t_intercambio* primer_intercambio(t_list* entrenadores_deadlock){
	t_entrenador_copia* primer_entrenador = list_get(entrenadores_deadlock, 0);

	char* pokemonARecibir = list_get(primer_entrenador->objetivos, 0);

	t_list* entrenadores_posibles = filtrar_entrenadores_con_pokemon(1, entrenadores_deadlock, pokemonARecibir);

	t_list* posibles_intercambios = obtener_posibles_intercambios(entrenadores_posibles, primer_entrenador, pokemonARecibir);
	list_destroy(entrenadores_posibles);

	t_list* intercambios = filtrar_dobles_intercambios(posibles_intercambios);

	t_intercambio_copia* mejor_intercambio = obtener_intercambio_mas_cercano(intercambios);
	list_destroy(intercambios);

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
			free(entrenador);
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
	list_sort(entrenador->adquiridos, strcasecmp);
	list_sort(entrenador->objetivos, strcasecmp);

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
	entrenador_intercambio->objetivos = list_duplicate(entrenador->objetivos);
	entrenador_intercambio->adquiridos = list_duplicate(entrenador->pokemones_adquiridos);
	entrenador_intercambio->posicion = entrenador->posicion;
	return entrenador_intercambio;
}





/*
 * Nosotros habiamos realizado toda la resolucion de deadlock con el codigo que esta arriba,
 * pero mas cerca de la entrega, nos dimos cuenta que una de las estadisticas que pedian
 * era deadlocks detectados.
 * Nosotros en un principio pensabamos que cantidad de deadlocks era igual que cantidad de intercambios
 * pero nos dimos cuenta de que esto no es asi.
 *
 * Por eso es que modelamos una nueva manera de resolver el deadlock, buscando primero detectar
 * todas las esperas circulares.
 * Si bien puede generar mayor numero de intercambios que el metodo anterior (en el que especificamente
 * buscabamos encontrar la menor serie de intercambios posibles), nos ayuda a encontrar puntualmente todas
 * las esperas circulares como pide el trabajo.
 *
 * De todas formas, el codigo anterior queda en la parte de arriba, y hasta podemos elegir que metodo usar
 * al cambiar el nombre de la llamada que hacemos.
 */


//--------------Deteccion de esperas circulares-------------------------------------------

int detectar_esperas_circulares(t_list* entrenadores_en_deadlock){

	if(list_is_empty(entrenadores_en_deadlock)){
		return 0;
	}

	int deadlocks_detectados = 0;

	t_entrenador_copia* primer_entrenador = list_get(entrenadores_en_deadlock, 0);

	t_list* espera_circular = list_create();
	list_add(espera_circular, primer_entrenador);

	t_list* espera_resultado = detectar_espera(espera_circular, primer_entrenador, primer_entrenador, entrenadores_en_deadlock);
	while(espera_resultado != NULL){
		printf("[Deadlock] Espera circular entre: [");
		for(int i = 0 ; i < list_size(espera_resultado) ; i++){
			t_entrenador_copia* e = list_get(espera_resultado, i);
			printf(" E%d ", e->entrenador->identificador);
		}
		printf("]\n");

		deadlocks_detectados++;
		//resuelvo
		resolver_espera_circular(espera_resultado);
		//saco de entrenadores_en_deadlock a los que ya no estan en deadlock
		preparar_entrenadores(entrenadores_en_deadlock);

		if(!list_is_empty(entrenadores_en_deadlock)){
			primer_entrenador = list_get(entrenadores_en_deadlock, 0);

			espera_circular = list_create();
			list_add(espera_circular, primer_entrenador);

			espera_resultado = detectar_espera(espera_circular, primer_entrenador, primer_entrenador, entrenadores_en_deadlock);
		}else{
			espera_resultado = NULL;
		}
	}

	printf("[Deadlock] Fin de deteccion\n");
	return deadlocks_detectados;
}

t_list* detectar_espera(t_list* espera_circular, t_entrenador_copia* candidato, t_entrenador_copia* entrenador_a_solucionar, t_list* entrenadores_en_deadlock){
	t_list* entrenadores_que_le_pueden_pedir = entrenadores_que_le_pueden_pedir_a(candidato, espera_circular, entrenadores_en_deadlock);

	if(list_is_empty(entrenadores_que_le_pueden_pedir)){
		return NULL;
	}

	for(int i = 0 ; i < list_size(entrenadores_que_le_pueden_pedir) ; i++){
		t_entrenador_copia* entrenador = list_get(entrenadores_que_le_pueden_pedir,i);
		if(entrenador_le_puede_pedir_algo_a(entrenador_a_solucionar, entrenador)){
			list_add(espera_circular, entrenador);
			return espera_circular;
		}
	}

	for(int i = 0 ; i < list_size(entrenadores_que_le_pueden_pedir) ; i++){
		t_entrenador_copia* entrenador = list_get(entrenadores_que_le_pueden_pedir,i);
		t_list* copia_espera = list_duplicate(espera_circular);
		list_add(espera_circular, entrenador);

		t_list* nueva_espera = detectar_espera(espera_circular, entrenador, entrenador_a_solucionar, entrenadores_en_deadlock);
		if(nueva_espera == NULL){
			espera_circular = copia_espera;
		}else{
			return nueva_espera;
		}
	}

	return NULL;
}

/*
 * Devuelve un listado de entrenadores, los cuales poseen como objetivo, alguno de los pokemones
 * que le sobran al candidato.
 *
 * Ademas, devuelve solo los que no estan en la espera circular ya.
 */
t_list* entrenadores_que_le_pueden_pedir_a(t_entrenador_copia* candidato, t_list* listado, t_list* entrenadores_deadlock){
	t_list* pueden_pedir = list_create();

	for(int i = 0 ; i < list_size(entrenadores_deadlock) ; i++){
		t_entrenador_copia* entrenador = list_get(entrenadores_deadlock, i);

		bool _es_entrenador(t_entrenador_copia* e){
			return e->entrenador->identificador == entrenador->entrenador->identificador;
		}

		if(!list_any_satisfy(listado, _es_entrenador)){
			for(int j = 0 ; j < list_size(entrenador->objetivos) ; j++){
				char* obj = list_get(entrenador->objetivos, j);
				if(entrenador_tiene_adquirido(candidato, obj)){
					list_add(pueden_pedir, entrenador);
					break;
				}
			}
		}
	}

	return pueden_pedir;
}

/*
 * Devuelve si es posible que el primer entrenador le pida algo al segundo,
 * osea si el primero tiene como objetivo algo que el segundo le sobra
 */
int entrenador_le_puede_pedir_algo_a(t_entrenador_copia* primero, t_entrenador_copia* segundo){
	for(int i = 0 ; i < list_size(primero->objetivos) ; i++){
		char* obj = list_get(primero->objetivos, i);
		if(entrenador_tiene_adquirido(segundo, obj)){
			return 1;
		}
	}
	return 0;
}


void resolver_espera_circular(t_list* espera_circular){

	int index_ultimo = list_size(espera_circular) - 1;

	t_entrenador_copia* entrenador = list_get(espera_circular, index_ultimo);
	t_entrenador_copia* proximo_entrenador = list_get(espera_circular, 0);
	char* ultimo_dado = que_especie_le_puedo_cambiar(proximo_entrenador->objetivos, entrenador);

	for(int i = index_ultimo ; i > 0  ; i--){
		t_entrenador_copia* entrenador = list_get(espera_circular, i);
		t_entrenador_copia* proximo_entrenador = list_get(espera_circular, i - 1);
		char* especie_a_obtener = que_especie_le_puedo_cambiar(entrenador->objetivos, proximo_entrenador);

		t_intercambio* intercambio = intercambio_create(entrenador->entrenador, proximo_entrenador->entrenador, ultimo_dado, especie_a_obtener);
		queue_push(cola_intercambios_deadlock, intercambio);

		//Interacmbio simbolico
		cambiar_pokemon(entrenador->adquiridos, ultimo_dado, especie_a_obtener);
		cambiar_pokemon(proximo_entrenador->adquiridos, especie_a_obtener, ultimo_dado);


		printf("[Deadlock] Nuevo intercambio programado entre entrenadores: %d y %d, para especies %s por %s\n",
						intercambio->entrenador->identificador, intercambio->entrenadorObjetivo->identificador,
						intercambio->pokemonADar, intercambio->pokemonARecibir);
		//ultimo_dado = especie_a_obtener;
	}

}

char* que_especie_le_puedo_cambiar(t_list* listado_pokemones, t_entrenador_copia* entrenador){
	for(int i = 0 ; i < list_size(listado_pokemones) ; i++){
		char* especie = list_get(listado_pokemones, i);
		if(entrenador_tiene_adquirido(entrenador, especie)){
			return especie;
		}
	}
	return "Hola";
}



void resolver_deadlock_nuevo(){
	loggear_inicio_deteccion_deadlock();

	t_list* entrenadores_copia = list_create();

	cola_intercambios_deadlock = queue_create();

	for(int i = 0 ; i < list_size(entrenadores) ; i++){
		t_entrenador* entrenador = list_get(entrenadores, i);
		t_entrenador_copia* e = entrenador_copia_create(entrenador);
		list_add(entrenadores_copia, e);
	}

	preparar_entrenadores(entrenadores_copia);

	deadlocks_detectados = detectar_esperas_circulares(entrenadores_copia);
	list_destroy(entrenadores_copia);

	loggear_resultado_deteccion_deadlock(deadlocks_detectados);
	printf("[Deadlock] Intercambios programados: %d\n", queue_size(cola_intercambios_deadlock));

	encolar_proximo_intercambio(1);
}






