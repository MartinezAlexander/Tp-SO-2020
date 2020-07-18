#include "entrenador.h"

void iniciarlizar_diccionario_catch(){
	mensajes_catch_pendientes = dictionary_create();
}

void actualizar_estadistica_entrenador(int id_entrenador, int ciclos_a_agregar){
	char* id = string_itoa(id_entrenador);
	int ciclos = (int) dictionary_get(diccionario_ciclos_entrenador, id);
	dictionary_put(diccionario_ciclos_entrenador, id, (void*)ciclos+ciclos_a_agregar);
	free(id);
}

/**
 * Esta es la funcion que me representa ejecutar un ciclo de un entrenador.
 * Se encarga de moverse hacia su objetivo, o realizar el envio de CATCH una vez que llego
 */
int ejecutar_entrenador(t_entrenador* entrenador){
	actualizar_estadistica_entrenador(entrenador->identificador, 1);

	int termine = mover_proxima_posicion(entrenador, entrenador->objetivo_actual->posicion);
	loggear_movimiento_entrenador(entrenador->identificador, entrenador->posicion);

	if(termine){
		enviar_catch(entrenador);
	}

	return termine;
}

/*
 * Ejecuta un ciclo de entrenador para realizar el intercambio de deadlock actual
 */
int ejecutar_entrenador_intercambio_deadlock(t_entrenador* entrenador){
	actualizar_estadistica_entrenador(entrenador->identificador, 1);

	int termine = mover_proxima_posicion(entrenador, entrenador->intercambio_actual->entrenadorObjetivo->posicion);
	loggear_movimiento_entrenador(entrenador->identificador, entrenador->posicion);

	if(termine){
		char* pokemon_a_recibir = entrenador->intercambio_actual->pokemonARecibir;
		char* pokemon_a_dar = entrenador->intercambio_actual->pokemonADar;
		cambiar_pokemon(entrenador->pokemones_adquiridos, pokemon_a_dar, pokemon_a_recibir);
		cambiar_pokemon(entrenador->intercambio_actual->entrenadorObjetivo->pokemones_adquiridos,
				pokemon_a_recibir, pokemon_a_dar);

		int ciclos_intercambio = 5;

		sleep(retardo_cpu * ciclos_intercambio);

		actualizar_estadistica_entrenador(entrenador->identificador, ciclos_intercambio);

		loggear_operacion_intercambio(entrenador->identificador,
						entrenador->intercambio_actual->entrenadorObjetivo->identificador, pokemon_a_dar, pokemon_a_recibir);

		actualizar_estado_entrenador(entrenador);
		actualizar_estado_entrenador(entrenador->intercambio_actual->entrenadorObjetivo);

		/*
		 * Al terminar el intercambio, como el hilo de mi otro entrenador sigue bloqueado,
		 * no le doy chance a que salga del while en caso de que quede en EXIT.
		 * Por eso es que chequeo esto y le mando el signal para que se desbloquee, y
		 * del otro lado me fijo si esta en EXIT para salir del while
		 */
		if(entrenador->intercambio_actual->entrenadorObjetivo->estado == EXIT){
			sem_post(&entrenador->intercambio_actual->entrenadorObjetivo->semaforo);
		}

		free(entrenador->intercambio_actual);
		entrenador->intercambio_actual = NULL;

		encolar_proximo_intercambio(0);
	}

	return termine;
}

void actualizar_estado_entrenador(t_entrenador* entrenador){
	if(cumplio_objetivo_entrenador(entrenador)){
		entrenador->estado = EXIT;
	}else if(entrenador_estado_deadlock(entrenador)){
		entrenador->estado = BLOCKED_DEADLOCK;
	}
}

void enviar_catch(t_entrenador* entrenador){
	sleep(retardo_cpu);

	actualizar_estadistica_entrenador(entrenador->identificador, 1);
	entrenador->estado = BLOCKED_BY_CATCH;
	//Cambie de estado, entonces habilito el semaforo
	//del planificador para que pueda mandar a ejecutar
	//a alguien mas, ya que yo me quedo esperando nada mas
	sem_post(&semaforo_planificacion);

	loggear_operacion_atrapar(entrenador->objetivo_actual);

	//Conecto con broker
	int socket = crear_conexion(ip_broker, puerto_broker);
	int conexion_exitosa = handshake(TEAM, BROKER, socket);
	if(conexion_exitosa <= 0){
		loggear_error_broker("envio de mensaje catch");
		//Comportamiento default: CATCH positivo
		resolver_caught_positivo(entrenador, 0);
	}else{
		//Envio mensaje
		t_catch_pokemon* mensaje_catch = catch_pokemon_create(entrenador->objetivo_actual->especie,
				entrenador->objetivo_actual->posicion.posicionX, entrenador->objetivo_actual->posicion.posicionY);
		//TODO 358 (128 direct, 230 indirect) bytes in 8 blocks are definitely lost in loss record 61 of 61 [PID: 11255]
		t_mensaje* mensaje = mensaje_simple_create(mensaje_catch, CATCH_POKEMON);

		int envio = enviar_mensaje(mensaje, socket);
		if(envio < 0){
			loggear_error_broker("envio de mensaje catch");
			//Comportamiento default: CATCH positivo
			resolver_caught_positivo(entrenador, 0);
		}else{
			int id = recibir_id(socket);
			printf("[Catch] Enviado CATCH => %s ; ID %d\n", mensaje_catch->pokemon->especie, id);
			//Agrego el id con mi entrenador al diccionario
			//TODO MEMORY LEAK
			char* key_id = string_itoa(id);
			dictionary_put(mensajes_catch_pendientes, key_id, entrenador);
		}
	}
}

void resolver_caught_positivo(t_entrenador* entrenador, int asincronico){

	printf("[Caught] Entrenador %d atrapo exitosamente al %s en [%d,%d]\n", entrenador->identificador,
			entrenador->objetivo_actual->especie, entrenador->objetivo_actual->posicion.posicionX,
			entrenador->objetivo_actual->posicion.posicionY);

	entrenador_atrapar_objetivo(entrenador);


	//Actualizo el estado del entrenador
	if(cumplio_objetivo_entrenador(entrenador)){
		entrenador->estado = EXIT;
		printf("[Estado] Entrenador %d finalizo su ejecucion\n", entrenador->identificador);
		//Puede darse el caso de que el entrenador entre en EXIT o DEADLOCK asincronicamente,
		//osea, cuando me llega la respuesta del CAUGHT (sin hacer comp. default)
		//Y cuando eso pase, el entrenador no podra salir del loop de ejecucion porque se
		//bloqueo anteriormente. Entonces debo mandar un signal para que el entrenador pueda
		//salir del while, sin ejecutar.
		if(asincronico) sem_post(&entrenador->semaforo);
	}else{
		if(entrenador_estado_deadlock(entrenador)){
			entrenador->estado = BLOCKED_DEADLOCK;
			printf("[Estado] Entrenador %d bloqueado por deadlock\n", entrenador->identificador);

			if(asincronico) sem_post(&entrenador->semaforo);
		}else{
			bloquear_entrenador(entrenador);
			printf("[Estado] Entrenador %d bloqueado\n", entrenador->identificador);
		}
	}

	/*
	 * En caso de que todos mis entrenadores hayan terminado la ejecucion
	 * de atrapar pokemones, debo hacer el signal para que el main continue y
	 * comience la deteccion de deadlock
	 */
	if(list_all_satisfy(entrenadores, entrenador_termino_ejecucion_normal)){
		sem_post(&semaforo_resolucion_deadlock);
	}
}

void resolver_caught_negativo(t_entrenador* entrenador){
	printf("[Caught] Entrenador %d no pudo atrapar su pokemon\n", entrenador->identificador);

	agregar_a_objetivos_globales(entrenador->objetivo_actual->especie);
	entrenador_resetear_objetivo(entrenador);
	bloquear_entrenador(entrenador);
}

void bloquear_entrenador(t_entrenador* entrenador){
	//Si puedo seguir atrapando, el entrenador
	//queda en estado bloqueado, asi la proxima
	//vez que aparezca un pokemon, este entrenador
	//este en los candidatos a ir a buscarlo
	entrenador->estado = BLOCKED;
	//Tambien al liberar el entrenador debo fijarme si tengo algun pokemon
	//en cola de espera y planificarlo o descartarlo segun corresponda.

	//Para esto debo chequear tambien que el pokemon en espera que procese no sea descartado
	//ya que en ese caso deberia tratar de procesar el proximo en la cola hasta que
	//pueda planificar a alguien o hasta que descarte todos los de la cola.
	while(entrenador->objetivo_actual == NULL && !queue_is_empty(cola_pokemones_en_espera)){
		procesar_pokemon_en_espera();
	}
}

/**
 * Comprueba si un entrenador tiene asignado un pokemon a atrapar (especie)
 */
int entrenador_tiene_objetivo(t_entrenador* entrenador, char* especie){
	if(entrenador->objetivo_actual != NULL){
		return string_equals_ignore_case(entrenador->objetivo_actual->especie, especie);
	}else
		return 0;
}

void agregar_a_objetivos_globales(char* especie){
	list_add(objetivo_global, especie);
	list_sort(objetivo_global, strcasecmp);
}

void sacar_de_objetivos_globales(char* especie, t_list* objetivos){
	if(!list_is_empty(objetivos)){
		for(int i = 0 ; i < list_size(objetivos) ; i++){
			char* p = list_get(objetivos, i);
			if(string_equals_ignore_case(especie, p)){
				list_remove(objetivos, i);
				break;
			}
		}
	}else{
		printf("[Error] Se intento sacar un pokemon de la Lista de objetivos globales pero estaba vacia\n");
	}
}

/**
 * Lleva a cabo el proximo movimiento del entrenador para llegar a su objetivo
 * (primero mueve en x, luego en y), devolviendo 1 en caso de que haya alcanzado
 * el mismo, y 0 si debe seguir moviendose
 */
int mover_proxima_posicion(t_entrenador* entrenador, t_posicion objetivo){
	int dirX = direccion_en_x(entrenador->posicion, objetivo);

	if(dirX == 0){
		int dirY = direccion_en_y(entrenador->posicion, objetivo);
		entrenador->posicion.posicionY += dirY;
	}else{
		entrenador->posicion.posicionX += dirX;
	}
	sleep(retardo_cpu);

	return movimientos_entre_posiciones(entrenador->posicion, objetivo) == 0;
}

//Recibe pokemones en un string separados por pipes y los asigna al entrenador
void asignar_pokemones(t_entrenador* entrenador, char* pokemones){
	//TODO MEMORY LEAK
	char** pokemones_adquiridos_array = string_split(pokemones, "|");
	entrenador->pokemones_adquiridos = array_to_list(pokemones_adquiridos_array);
	free(pokemones_adquiridos_array);
}

t_entrenador* entrenador_create(char* posicion, char* objetivos, char* adquiridos, int identificador, double estimacion_inicial ){
	//TODO 358 (128 direct, 230 indirect) bytes in 8 blocks are definitely lost in loss record 61 of 61 [PID: 11255]
	t_entrenador* entrenador = malloc(sizeof(t_entrenador));

	//TODO [ML] Nos tira leak pero tres lineas abajo no y hacemos lo mismo..?
	char** posiciones_separadas = string_split(posicion, "|");
	entrenador->posicion = posicion_create( atoi(posiciones_separadas[0]), atoi(posiciones_separadas[1]));
	//free(posiciones_separadas);
	free_string_array(posiciones_separadas);
	//TODO MEMORY LEAK
	char** objetivos_array = string_split(objetivos, "|");
	entrenador->objetivos = array_to_list(objetivos_array);
	//free(objetivos_array);
	//free_string_array(objetivos_array);

	//Asigno si veo que tiene pokemones de entrada
	if(adquiridos != NULL){
		asignar_pokemones(entrenador, adquiridos);
	}else{
		entrenador->pokemones_adquiridos = list_create();
	}

	entrenador->estado = NEW;
	entrenador->identificador = identificador;

	entrenador->objetivo_actual = NULL;
	entrenador->intercambio_actual = NULL;

	entrenador->estado_sjf = malloc(sizeof(estado_sjf));
	entrenador->estado_sjf->ultima_rafaga = 0;
	entrenador->estado_sjf->ultima_estimacion = estimacion_inicial;
	entrenador->estado_sjf->empezo_a_ejecutar = 0;

	char* id_entrenador = string_itoa(entrenador->identificador);
	dictionary_put(diccionario_ciclos_entrenador, id_entrenador, 0);
	free(id_entrenador);

	actualizar_estado_entrenador(entrenador);

	return entrenador;
}

//Esta funcion fue necesaria ya que la funcion que nos daba las commons para
//leer un array del config, no nos indicaba si alguna posicion tenia NULL entre sus
//tokens. Nosotros necesitabamos saber esto para cuando leemos los pokemones
//obtenidos, ya que el config puede venir sin pokemones obtenidos.
//Tuvimos que hacer esta funcion manualmente entonces

//Toma un string del formato "Token,Token,Token"
//y devuelve una list con cada item.
//El item sera NULL si no hay token entre
//dos separadores
t_list* separar(char* listado){
	t_list* listado_separado = list_create();

	int len_no_brackets = strlen(listado) - 2;
	//TODO MEMORY LEAK
	char* listado_sin_brackets = string_substring(listado, 1, len_no_brackets);

	char* token;
	while((token = strsep(&listado_sin_brackets, ",")) != NULL){
		if(!strcasecmp(token, "")){
			list_add(listado_separado, NULL);
		}else{
			list_add(listado_separado, token);
		}
	}

	free(listado_sin_brackets);

	return listado_separado;
}

t_list* leer_entrenadores(t_config* config, double estimacion_inicial){
	char** posiciones_entrenadores = config_get_array_value(config, "POSICIONES_ENTRENADORES");
	char* listado_pokemones_adquiridos = config_get_string_value(config, "POKEMON_ENTRENADORES");
	//TODO MEMORY LEAK
	char** objetivos_entrenadores = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
	//TODO [ML] Memory Leak de las commons??

	int numero_posiciones = array_cantidad_de_elementos(posiciones_entrenadores);
	int numero_obj_entrenadores = array_cantidad_de_elementos(objetivos_entrenadores);

	t_list* pokemones_entrenadores = separar(listado_pokemones_adquiridos);

	//Error si no coinciden las cantidades
	if(numero_posiciones != numero_obj_entrenadores){
		printf("[Config] Error: no coindiden las cantidades de pos-obj de entrenadores en config!");
	}

	t_list* entrenadores = list_create();
	diccionario_ciclos_entrenador = dictionary_create();
	iniciarlizar_diccionario_catch();

	for(int i = 0 ; i < numero_posiciones ; i++){
		char* pokemones = list_get(pokemones_entrenadores, i);

		t_entrenador* entrenador = entrenador_create(posiciones_entrenadores[i],
				objetivos_entrenadores[i], pokemones, (i+1), estimacion_inicial);

		list_add(entrenadores, entrenador);
	}

	//free(posiciones_entrenadores);
	free_string_array(posiciones_entrenadores);
	//free_string_array(objetivos_entrenadores);
	free(listado_pokemones_adquiridos);

	list_destroy(pokemones_entrenadores);

	/*
	 * En caso de que todos mis entrenadores empiecen en EXIT o DEADLOCK,
	 * debo hacer el signal para que el main continue y
	 * comience la deteccion de deadlock
	 */
	if(list_all_satisfy(entrenadores, entrenador_termino_ejecucion_normal)){
		sem_post(&semaforo_resolucion_deadlock);
	}

	return entrenadores;
}


int entrenador_en_ejecucion(t_entrenador *entrenador)
{
	return(entrenador->estado == EXEC);
}

int entrenador_en_deadlock(t_entrenador *entrenador)
{
	return(entrenador->estado == BLOCKED_DEADLOCK);
}

int entrenador_termino_ejecucion_normal(t_entrenador *entrenador)
{
	return(entrenador->estado == BLOCKED_DEADLOCK || entrenador->estado == EXIT);
}

void entrenador_atrapar_objetivo(t_entrenador* entrenador){
	char* nuevo_pokemon = entrenador->objetivo_actual->especie;
	list_add(entrenador->pokemones_adquiridos, nuevo_pokemon);

	entrenador_resetear_objetivo(entrenador);
}

void entrenador_resetear_objetivo(t_entrenador* entrenador){
	entrenador->objetivo_actual = NULL;
}

int cumplio_objetivo_entrenador(t_entrenador* entrenador){
	//Si no tienen mismo tamaño ya se que no son iguales
	if(list_size(entrenador->objetivos) != list_size(entrenador->pokemones_adquiridos)) return 0;

	//La ordeno para poder compararlas
	list_sort(entrenador->objetivos, strcasecmp);
	list_sort(entrenador->pokemones_adquiridos, strcasecmp);

	//Comparo con los objetivos
	for(int i = 0 ; i < list_size(entrenador->pokemones_adquiridos) ; i++){
		char* pk1 = list_get(entrenador->pokemones_adquiridos, i);
		char* pk2 = list_get(entrenador->objetivos, i);

		if(!string_equals_ignore_case(pk1,pk2)) return 0;
	}

	return 1;
}

//En realidad esta funcion no chequea si el entrenador esta en deadlock,
//pero en el contexto en el que se usa si logra decirme el estado.
//Basicamente cuando llamo a esta funcion, primero me fijo si
//el entrenador necesita seguir atrapando pokemones,
//en caso de que no necesite mas llamo a esta funcion que se fija
//el tamaño de las listas. Si tengo la misma cantidad de adquiridos que
//de pokemones que necesito (sumado a que anteriormente me fije si habia cumplido
//los objetivos o no) puedo decir que estoy en deadlock
int entrenador_estado_deadlock(t_entrenador* entrenador){
	return list_size(entrenador->objetivos) == list_size(entrenador->pokemones_adquiridos);
}

t_entrenador* obtener_entrenador_mas_cercano(t_list* entrenadores, t_posicion posicion){
	int index_mas_cercano = 0;
	//Ya empieza como la distancia del primer entrenador que aparece
	t_posicion posicion_entrenador = ((t_entrenador*)list_get(entrenadores, 0))->posicion;
	int distancia_mas_corta = movimientos_entre_posiciones(posicion, posicion_entrenador);

	//Comparo la cantidad de movimientos que necesito para llegar a pos.
	for(int i = 1 ; i < list_size(entrenadores) ; i++){
		t_posicion posicion_entrenador = ((t_entrenador*)list_get(entrenadores, i))->posicion;
		int distancia = movimientos_entre_posiciones(posicion, posicion_entrenador);

		//En caso de empate se queda el que ya estaba (el primero)
		if(distancia < distancia_mas_corta){
			distancia_mas_corta = distancia;
			index_mas_cercano = i;
		}
	}

	return list_get(entrenadores, index_mas_cercano);
}

void entrenador_destroy(t_entrenador* e){
	//TODO Address 0x423ebf0 is 0 bytes inside a block of size 9 free'd [PID: 11255]
	list_destroy_and_destroy_elements(e->pokemones_adquiridos, free);
	//TODO invalid free
	list_destroy_and_destroy_elements(e->objetivos, free);

	free(e->estado_sjf);

	if(e->objetivo_actual != NULL){
		printf("[ERROR] Entrenador sigue teniendo objetivo actual al finalizar el programa\n");
	}

	sem_destroy(&e->semaforo);
}

void entrenadores_destroy(){
	dictionary_destroy(mensajes_catch_pendientes);

	list_destroy_and_destroy_elements(entrenadores, entrenador_destroy);
}
