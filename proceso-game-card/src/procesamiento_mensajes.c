#include "procesamiento_mensajes.h"

//TODO Todas las ejecuciones
void ejecutar_new(t_mensaje* mensaje){
	//1. Verificar si el Pokemon existe dentro de nuestro Filesystem (Si no existe->Se crea)
	//2. Verificar si se puede abrir el archivo (Si no->Se reintenta cada X seg)
	//3. Verificar si las posiciones ya existen dentro del archivo (Si existen se suma la
	//cantidad pasada, si no, se agrega una linea
	//4. Esperar la cantidad de segundos definidos por archivo de configuracion
	//5. Cerrar el archivo
	//6. Conectarse y enviar al broker APPEARED_POKEMON (ID recibido, poke, posicion)
	// Si no se puede conectar, se loggea y continua la ejecucion
}

void ejecutar_catch(t_mensaje* mensaje){
	//1. Verificar si el Pokemon existe dentro de nuestro Filesystem (Si no existe->se
	//informa el error
	//2. Verificar si se puede abrir el archivo (Si no->Se reintenta cada X seg)
	//3. Verificar si las posiciones ya existen dentro del archivo (Si no->se informa error)
	//4. SI la cantidad = 1, se elimina la linea. Sino se decrementa la cantidad en 1
	//5. Esperar la cantidad de segundos definidos por archivo de configuracion
	//6. Cerrar el archivo
	//7. Conectarse y enviar al broker el resultado (ID recibido, resultado)
	// Si no se puede conectar, se loggea y continua la ejecucion
}

void ejecutar_get(t_mensaje* mensaje){
	//1. Verificar si el Pokemon existe dentro de nuestro Filesystem (Si no existe->se
	//informa el mensaje sin posiciones ni cantidades (???)
	//2. Verificar si se puede abrir el archivo (Si no->Se reintenta cada X seg)
	//3. Obtener todas las posiciones y cantidades del Pokemon requerido
	//4. Esperar la cantidad de segundos definidos por archivo de configuracion
	//5. Cerrar el archivo
	//6. Si se encontro al menos 1 posicion, conectarse y enviar al broker
	// LOCALIZED_POKEMON (ID recibido, poke, lista de posiciones y cant. por posicion)
	// Si no se puede conectar, se loggea y continua la ejecucion
}
