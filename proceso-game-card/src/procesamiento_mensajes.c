#include "procesamiento_mensajes.h"

//TODO Todas las ejecuciones
void ejecutar_new(t_new_pokemon* new_pokemon, int id) {
	//Obtengo el archivo del pokemon que necesito. Si no existe se crea.
	pokemon_file* archivo_pokemon = obtener_pokemon(
			new_pokemon->pokemon->especie);
	abrir_archivo(archivo_pokemon);
	agregar_pokemon(archivo_pokemon, new_pokemon->pokemon->posicion,
			new_pokemon->cantidad);

	//Espero x segundos para simular el acceso al disco
	sleep(tiempo_retardo_operacion);
	cerrar_archivo(archivo_pokemon);

	//Envio mensaje APPEARED al broker
	int socket = crear_conexion(ip_broker, puerto_broker);
	if (socket >= 0) {
		t_appeared_pokemon* appeared = appeared_pokemon_create(
				new_pokemon->pokemon->especie,
				new_pokemon->pokemon->posicion.posicionX,
				new_pokemon->pokemon->posicion.posicionY);
		t_mensaje* mensaje = mensaje_con_id_correlativo_create(appeared,
				APPEARED_POKEMON, id);
		int envio = enviar_mensaje(mensaje, socket);
		recibir_id(socket);
		liberar_conexion(socket);
	} else {
		//TODO loggeo error
	}
}

void ejecutar_catch(t_catch_pokemon* pokemon, int id) {
	//1. Verificar si el Pokemon existe dentro de nuestro Filesystem (Si no existe->se informa el error
	pokemon_file* archivo_pokemon = existe_pokemon(pokemon->pokemon->especie);
	t_caught_pokemon resultado;
	if (archivo_pokemon != NULL){
		resultado = 1;
	}else{
		resultado = 0;
	}

	//2. Verificar si se puede abrir el archivo (Si no->Se reintenta cada X seg)
	abrir_archivo(archivo_pokemon);

	//3. Verificar si las posiciones ya existen dentro del archivo (Si no->se informa error)
	if(existe_posicion(pokemon->pokemon->posicion)==1){
		 resultado = 1;
	}else{
		resultado = 0;
	}
	//4. SI la cantidad = 1, se elimina la linea. Sino se decrementa la cantidad en 1
	incrementar_cantidad(pokemon->pokemon->posicion);

	//5. Esperar la cantidad de segundos definidos por archivo de configuracion
	sleep(tiempo_retardo_operacion);

	//6. Cerrar el archivo
	cerrar_archivo(archivo_pokemon);

	//7. Conectarse y enviar al broker el resultado (ID recibido, resultado)
	int socket = crear_conexion(ip_broker, puerto_broker);
	if (socket >= 0) {
		//TODO CAUGHT NO CATCH

		t_mensaje* mensaje = mensaje_con_id_correlativo_create(resultado,CAUGHT_POKEMON, id);
		enviar_mensaje(mensaje, socket);
		recibir_id(socket);
		liberar_conexion(socket);
	} else {
		// TODO Si no se puede conectar, se loggea y continua la ejecucion
	}

}

void ejecutar_get(t_get_pokemon* pokemon, int id) {
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
