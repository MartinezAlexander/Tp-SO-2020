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
		t_mensaje* mensaje = mensaje_con_id_correlativo_create(appeared,APPEARED_POKEMON, id);
		enviar_mensaje(mensaje, socket);
		recibir_id(socket);
		liberar_conexion(socket);
	} else {
		//loggeo error
		loggear_conexion(socket);
	}
}

void ejecutar_catch(t_catch_pokemon* pokemon, int id) {
	//1. Verificar si el Pokemon existe dentro de nuestro Filesystem (Si no existe->se informa el error

	//PREGUNTAR EL SABADO: 'Se informa error' si no existe se loggea o se manda cuaght negativo??

	pokemon_file* archivo_pokemon = existe_pokemon(pokemon->pokemon->especie);

	t_caught_pokemon* caught_respuesta;

	if (archivo_pokemon == NULL) {
		caught_respuesta = caught_pokemon_create(0);
	} else {
		//2. Verificar si se puede abrir el archivo (Si no->Se reintenta cada X seg)
		abrir_archivo(archivo_pokemon);

		//3. Verificar si las posiciones ya existen dentro del archivo (Si no->se informa error)
		caught_respuesta = caught_pokemon_create(existe_posicion(archivo_pokemon, pokemon->pokemon->posicion));

		//4. SI la cantidad = 1, se elimina la linea. Sino se decrementa la cantidad en 1
		incrementar_cantidad(archivo_pokemon, pokemon->pokemon->posicion);

		//5. Esperar la cantidad de segundos definidos por archivo de configuracion
		sleep(tiempo_retardo_operacion);

		//6. Cerrar el archivo
		cerrar_archivo(archivo_pokemon);
	}

	//7. Conectarse y enviar al broker el resultado (ID recibido, resultado)
	int socket = crear_conexion(ip_broker, puerto_broker);
	if (socket >= 0) {
		t_mensaje* mensaje = mensaje_con_id_correlativo_create((void*) caught_respuesta, CAUGHT_POKEMON, id);
		enviar_mensaje(mensaje, socket);
		recibir_id(socket);
		liberar_conexion(socket);
	} else {
		// loggeao que no se pudo conectar
		loggear_conexion(socket);
	}

}

void ejecutar_get(t_get_pokemon* pokemon, int id) {
	t_localized_pokemon* localized_respuesta;
	//1. Verificar si el Pokemon existe dentro de nuestro Filesystem
	pokemon_file* archivo_pokemon = existe_pokemon(pokemon->nombre);

	if (archivo_pokemon == NULL) {
		//(Si no existe->se informa el mensaje sin posiciones ni cantidades (???)
		localized_respuesta = localized_pokemon_create(pokemon->nombre, 0);
	} else {
		//2. Verificar si se puede abrir el archivo (Si no->Se reintenta cada X seg)
		abrir_archivo(archivo_pokemon);

		//3. TODO Obtener todas las posiciones del Pokemon requerido
		t_list* pokemon_posiciones = obtener_posiciones(pokemon->nombre);
		localized_respuesta = localized_pokemon_create(pokemon->nombre,pokemon_posiciones);

		//4. Esperar la cantidad de segundos definidos por archivo de configuracion
		sleep(tiempo_retardo_operacion);

		//5. Cerrar el archivo
		cerrar_archivo(archivo_pokemon);
	}

	//6. Si se encontro al menos 1 posicion, conectarse y enviar al broker
	//  [no hay que enviar las cantidades esta mal el enunciado])

	int socket = crear_conexion(ip_broker, puerto_broker);
	if (socket >= 0) {
		t_mensaje* mensaje = mensaje_con_id_correlativo_create(
				(void*) localized_respuesta, LOCALIZED_POKEMON, id);
		enviar_mensaje(mensaje, socket);
		recibir_id(socket);
		liberar_conexion(socket);
	} else {
		// loggeo que no se pudo conectar
		loggear_conexion(socket);
	}
}
