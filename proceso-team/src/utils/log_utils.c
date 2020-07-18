#include "log_utils.h"


//TODO [Log]: 1. Cambio de un entrenador de cola de planificacion (indicando razon porque)
	//Preguntar a que se refiere con cambio
	//Cuando encolo, cuando pongo alguien en ejecucion, cuando saco a alguien???
void loggear_planificacion(){

}
//2. Movimiento de un entrenador (indicando la ubicacion a la que se movio)
void loggear_movimiento_entrenador(int identificador_entrenador, t_posicion posicion){
	printf("[Ejecucion] Entrenador de id: %d se movio a la posicion [%d,%d]\n",
			identificador_entrenador, posicion.posicionX, posicion.posicionY);

	log_info(logger, "[Ejecucion] Entrenador de id: %d se movio a la posicion [%d,%d]",
			identificador_entrenador, posicion.posicionX, posicion.posicionY);
}
//3. Operacion de atrapar (indicando la ubicacion y el pokemon a atrapar)
void loggear_operacion_atrapar(t_pokemon* pokemon){
	printf("[Ejecucion] Inicio operacion CATCH de un: %s, ubicado en: [%d,%d]\n",
			pokemon->especie, pokemon->posicion.posicionX, pokemon->posicion.posicionY);

	log_info(logger, "[Ejecucion] Inicio operacion CATCH de un: %s, ubicado en: [%d,%d]",
			pokemon->especie, pokemon->posicion.posicionX, pokemon->posicion.posicionY);
}
//4. Operacion de intercambio (indicando entrenadores involucrados)
void loggear_operacion_intercambio(int entrenador, int otro_entrenador, char* especie, char* otra_especie){
	printf("[Deadlock] Se realizo intercambio de pokemones entre entrenadores %d y %d: %s por %s\n", entrenador, otro_entrenador, especie, otra_especie);

	log_info(logger, "[Deadlock] Se realizo intercambio de pokemones entre entrenadores %d y %d: %s por %s", entrenador, otro_entrenador, especie, otra_especie);
}
//5. Inicio de algoritmo de deteccion de deadlock
void loggear_inicio_deteccion_deadlock(){
	printf("[Deadlock] Comienza la deteccion de deadlock\n");

	log_info(logger, "[Deadlock] Comienza la deteccion de deadlock");
}
//6. Resultado de algoritmo de deteccion de deadlock
void loggear_resultado_deteccion_deadlock(int deadlocks){
	printf("[Deadlock] Se detectaron %d deadlocks para resolver\n", deadlocks);

	log_info(logger, "[Deadlock] Se detectaron %d deadlocks para resolver", deadlocks);
}
//7. Llegada de un mensaje (indicando el tipo del mismo y sus datos)
void loggear_nuevo_mensaje(t_mensaje* mensaje){
	char* mensaje_string = mensaje_to_string(mensaje);

	printf("%s\n", mensaje_string);

	log_info(logger, mensaje_string);
	free(mensaje_string);
}

//8. Resultado del Team (especificado anteriormente)
void loggear_resultado_team(){
	int ciclos_cpu_totales = 0;

	for(int i = 0 ; i < dictionary_size(diccionario_ciclos_entrenador) ; i++){

		char* key_entrenador = string_itoa(i+1);
		int ciclos = (int)dictionary_get(diccionario_ciclos_entrenador, key_entrenador);
		free(key_entrenador);
		ciclos_cpu_totales += ciclos;
	}


	printf("[Team] Resultado proceso team:\n");
	log_info(logger, "[Team] Resultado proceso team:");

	printf("[Team] Ciclos de cpu totales: %d\n", ciclos_cpu_totales);
	log_info(logger, "[Team] Ciclos de cpu totales: %d", ciclos_cpu_totales);

	printf("[Team] Cambios de contexto realizados: %d\n", cambios_de_contexto);
	log_info(logger, "[Team] Cambios de contexto realizados: %d", cambios_de_contexto);

	printf("[Team] Ciclos de cpu por entrenador:\n");
	log_info(logger, "[Team] Ciclos de cpu por entrenador:");

	for(int i = 0 ; i < dictionary_size(diccionario_ciclos_entrenador) ; i++){
		char* key_entrenador = string_itoa(i+1);
		int ciclos = (int)dictionary_get(diccionario_ciclos_entrenador, key_entrenador);
		free(key_entrenador);

		printf("[Team] Entrenador %d => %d ciclos de cpu\n", i+1, ciclos);
		log_info(logger, "[Team] Entrenador %d => %d ciclos de cpu", i+1, ciclos);
	}

	printf("[Team] Deadlocks producidos y resueltos: %d\n", deadlocks_detectados);
	log_info(logger, "[Team] Deadlocks producidos y resueltos: %d", deadlocks_detectados);

}

void loggear_error_broker(char* tipo_operacion){
	printf("[Conexion] Error de comunicacion con el proceso Broker en %s: se realizara el comportamiento default\n", tipo_operacion);

	log_info(logger, "[Conexion] Error de comunicacion con el proceso Broker en %s: se realizara el comportamiento default", tipo_operacion);
}

void loggear_inicio_reintento_broker(){
	printf("[Conexion] Iniciando reintento de conexion con el proceso Broker\n");

	log_info(logger, "[Conexion] Iniciando reintento de conexion con el proceso Broker");
}

void loggear_resultado_reintento_broker(int resultado){
	if(resultado){
		printf("[Conexion] Finalizo el reintento de conexion con el proceso Broker: conexion exitosa\n");
		log_info(logger, "[Conexion] Finalizo el reintento de conexion con el proceso Broker: conexion exitosa");
	}else{
		printf("[Conexion] Finalizo el reintento de conexion con el proceso Broker: no se pudo reconectar\n");
		log_info(logger, "[Conexion] Finalizo el reintento de conexion con el proceso Broker: no se pudo reconectar");
	}

}
