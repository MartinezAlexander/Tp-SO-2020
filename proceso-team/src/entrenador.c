#include "entrenador.h"

void iniciarlizar_diccionario_catch(){
	mensajes_catch_pendientes = dictionary_create();
}

void actualizar_estadistica_entrenador(int id_entrenador){
	char* id = string_itoa(id_entrenador);
	int ciclos = (int) dictionary_get(diccionario_ciclos_entrenador, id);
	dictionary_put(diccionario_ciclos_entrenador, id, (void*)ciclos+1);
	free(id);
}

int ejecutar_entrenador(t_entrenador* entrenador){
	actualizar_estadistica_entrenador(entrenador->identificador);

	int termine = mover_proxima_posicion(entrenador);
	loggear_movimiento_entrenador(entrenador->identificador, entrenador->posicion);

	if(termine){
		enviar_catch(entrenador);
	}

	return termine;
}

void enviar_catch(t_entrenador* entrenador){
	sleep(retardo_cpu);
	actualizar_estadistica_entrenador(entrenador->identificador);
	entrenador->estado = BLOCKED_BY_CATCH;
	//Cambie de estado, entonces habilito el semaforo
	//del planificador para que pueda mandar a ejecutar
	//a alguien mas, ya que yo me quedo esperando nada mas
	sem_post(&semaforo_planificacion);

	loggear_operacion_atrapar(entrenador->objetivo_actual);

	//Conecto con broker
	int socket = crear_conexion(ip_broker, puerto_broker);
	if(socket < 0){
		loggear_error_broker("envio de mensaje catch");
		//Comportamiento default: CATCH positivo
		resolver_caught_positivo(entrenador);
	}else{
		//Envio mensaje
		t_catch_pokemon* mensaje_catch = catch_pokemon_create(entrenador->objetivo_actual->especie,
				entrenador->objetivo_actual->posicion.posicionX, entrenador->objetivo_actual->posicion.posicionY);
		t_mensaje* mensaje = mensaje_simple_create(mensaje_catch, CATCH_POKEMON);

		int envio = enviar_mensaje(mensaje, socket);
		if(envio < 0){
			loggear_error_broker("envio de mensaje catch");
			//Comportamiento default: CATCH positivo
			resolver_caught_positivo(entrenador);
		}else{
			int id = recibir_id(socket);
			printf("-Enviado el catch, y recibido id de mensaje: %d -\n", id);
			//Agrego el id con mi entrenador al diccionario
			char* key_id = string_itoa(id);
			dictionary_put(mensajes_catch_pendientes, key_id, entrenador);
			printf("TODO OK\n");
		}
	}
}

void resolver_caught_positivo(t_entrenador* entrenador){

	printf("Entrenador %d atrapo exitosamente al %s en [%d,%d]\n", entrenador->identificador,
			entrenador->objetivo_actual->especie, entrenador->objetivo_actual->posicion.posicionX,
			entrenador->objetivo_actual->posicion.posicionY);

	entrenador_atrapar_objetivo(entrenador);

	//Actualizo el estado del entrenador
	if(cumplio_objetivo_entrenador(entrenador)){
		entrenador->estado = EXIT;
	}else{
		if(entrenador_estado_deadlock(entrenador))
			entrenador->estado = BLOCKED_DEADLOCK;
		else
			entrenador->estado = BLOCKED;
			//Si puedo seguir atrapando, el entrenador
			//queda en estado bloqueado, asi la proxima
			//vez que aparezca un pokemon, este entrenador
			//este en los candidatos a ir a buscarlo
	}
}

void resolver_caught_negativo(t_entrenador* entrenador){
	printf("Entrenador %d no pudo atrapar su pokemon\n", entrenador->identificador);

	agregar_a_objetivos_globales(entrenador->objetivo_actual->especie);
	entrenador_resetear_objetivo(entrenador);
	entrenador->estado = BLOCKED;
}

void agregar_a_objetivos_globales(char* especie){
	list_add(objetivo_global, especie);
	list_sort(objetivo_global, strcmp);
}

void sacar_de_objetivos_globales(char* especie, t_list* objetivos){
	for(int i = 0 ; i < list_size(objetivos) ; i++){
		char* p = list_get(objetivos, i);

		if(string_equals_ignore_case(especie, p)){
			list_remove(objetivos, i);
			break;
		}
	}
}

/**
 * Lleva a cabo el proximo movimiento del entrenador para llegar a su objetivo
 * (primero mueve en x, luego en y), devolviendo 1 en caso de que haya alcanzado
 * el mismo, y 0 si debe seguir moviendose
 */
int mover_proxima_posicion(t_entrenador* entrenador){
	int dirX = direccion_en_x(entrenador->posicion, entrenador->objetivo_actual->posicion);

	if(dirX == 0){
		int dirY = direccion_en_y(entrenador->posicion, entrenador->objetivo_actual->posicion);
		entrenador->posicion.posicionY += dirY;
	}else{
		entrenador->posicion.posicionX += dirX;
	}
	sleep(retardo_cpu);

	return movimientos_entre_posiciones(entrenador->posicion, entrenador->objetivo_actual->posicion) == 0;
}

t_entrenador* entrenador_create(char* posicion, char* pokemones, char* objetivos, int identificador, double estimacion_inicial ){
	t_entrenador* entrenador = malloc(sizeof(t_entrenador));

	char** posiciones_separadas = string_split(posicion, "|");
	entrenador->posicion = *posicion_create( atoi(posiciones_separadas[0]) , atoi(posiciones_separadas[1]) );
	free(posiciones_separadas);

	char** pokemones_adquiridos_array = string_split(pokemones, "|");
	entrenador->pokemones_adquiridos = array_to_list(pokemones_adquiridos_array);
	free(pokemones_adquiridos_array);

	char** objetivos_array = string_split(objetivos, "|");
	entrenador->objetivos = array_to_list(objetivos_array);
	free(objetivos_array);

	entrenador->estado = NEW;
	entrenador->identificador = identificador;

	entrenador->objetivo_actual = NULL;

	entrenador->estado_sjf = malloc(sizeof(estado_sjf));
	entrenador->estado_sjf->ultima_rafaga = 0;
	entrenador->estado_sjf->ultima_estimacion = estimacion_inicial;

	char* id_entrenador = string_itoa(entrenador->identificador);
	dictionary_put(diccionario_ciclos_entrenador, id_entrenador, 0);
	free(id_entrenador);

	return entrenador;
}


t_list* leer_entrenadores(t_config* config, double estimacion_inicial){
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
	diccionario_ciclos_entrenador = dictionary_create();
	iniciarlizar_diccionario_catch();

	for(int i = 0 ; i < numero_posiciones ; i++){
		list_add(entrenadores,entrenador_create(posiciones_entrenadores[i],
				pokemones_entrenadores[i],
				objetivos_entrenadores[i], (i+1), estimacion_inicial));
	}

	free(posiciones_entrenadores);
	free(pokemones_entrenadores);
	free(objetivos_entrenadores);

	return entrenadores;
}


int entrenador_en_ejecucion(t_entrenador *entrenador)
{
	return(entrenador->estado == EXEC);
}

void entrenador_atrapar_objetivo(t_entrenador* entrenador){
	char* nuevo_pokemon = entrenador->objetivo_actual->especie;
	list_add(entrenador->pokemones_adquiridos, nuevo_pokemon);

	sacar_de_objetivos_globales(nuevo_pokemon, objetivo_global);
	entrenador_resetear_objetivo(entrenador);
}

void entrenador_resetear_objetivo(t_entrenador* entrenador){
	entrenador->objetivo_actual = NULL;
}

int cumplio_objetivo_entrenador(t_entrenador* entrenador){
	//Si no tienen mismo tamaño ya se que no son iguales
	if(list_size(entrenador->objetivos) != list_size(entrenador->pokemones_adquiridos)) return 0;

	//La ordeno para poder compararlas
	list_sort(entrenador->objetivos, strcmp);
	list_sort(entrenador->pokemones_adquiridos, strcmp);

	//Comparo con los objetivos
	for(int i = 0 ; i < list_size(entrenador->pokemones_adquiridos) ; i++){
		char* pk1 = list_get(entrenador->pokemones_adquiridos, i);
		char* pk2 = list_get(entrenador->objetivos, i);

		if(string_equals_ignore_case(pk1,pk2) != 1) return 0;
	}

	return 1;
}

int entrenador_estado_deadlock(t_entrenador* entrenador){
	return list_size(entrenador->objetivos) == list_size(entrenador->pokemones_adquiridos);
}

t_entrenador* obtener_entrenador_mas_cercano(t_list* entrenadores, t_posicion posicion){
	int index_mas_cercano;
	//Ya empieza como la maxima distancia posible
	int distancia_mas_corta = posicion.posicionX + posicion.posicionY;

	//Comparo la cantidad de movimientos que necesito para llegar a pos.
	for(int i = 0 ; i < list_size(entrenadores) ; i++){
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
