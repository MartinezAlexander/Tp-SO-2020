#include "log_utils.h"


//TODO log: 1. Cambio de un entrenador de cola de planificacion (indicando razon porque)
	//Preguntar a que se refiere con cambio
	//Cuando encolo, cuando pongo alguien en ejecucion, cuando saco a alguien???
void loggear_planificacion(){

}
//2. Movimiento de un entrenador (indicando la ubicacion a la que se movio)
void loggear_movimiento_entrenador(int identificador_entrenador, t_posicion posicion){
	log_info(logger, "[Ejecucion] Entrenador de id: %d se movio a la posicion [%d,%d]",
			identificador_entrenador, posicion.posicionX, posicion.posicionY);
}
//3. Operacion de atrapar (indicando la ubicacion y el pokemon a atrapar)
void loggear_operacion_atrapar(t_pokemon* pokemon){
	log_info(logger, "[Ejecucion] Inicio operacion CATCH de un: %s, ubicado en: [%d,%d]",
			pokemon->especie, pokemon->posicion.posicionX, pokemon->posicion.posicionY);
}
//TODO log: 4. Operacion de intercambio (indicando entrenadores involucrados)
void loggear_operacion_intercambio(){

}
//TODO log: 5. Inicio de algoritmo de deteccion de deadlock
void loggear_inicio_deteccion_deadlock(){

}
//TODO log: 6. Resultado de algoritmo de deteccion de deadlock
void loggear_resultado_deteccion_deadlock(){

}
//7. Llegada de un mensaje (indicando el tipo del mismo y sus datos)
void loggear_nuevo_mensaje(t_mensaje* mensaje){
	log_info(logger, mensaje_to_string(mensaje));
}

//TODO log: 8. Resultado del Team (especificado anteriormente)
void loggear_resultado_team(){

}

void loggear_error_broker(char* tipo_operacion){
	log_info(logger, "[Conexion] Error de comunicacion con el proceso Broker en %s: se realizara el comportamiento default", tipo_operacion);
}

void loggear_inicio_reintento_broker(){
	log_info(logger, "[Conexion] Iniciando reintento de conexion con el proceso Broker");
}

void loggear_resultado_reintento_broker(int resultado){
	if(resultado){
		log_info(logger, "[Conexion] Finalizo el reintento de conexion con el proceso Broker: conexion exitosa");
	}else{
		log_info(logger, "[Conexion] Finalizo el reintento de conexion con el proceso Broker: no se pudo reconectar");
	}

}
