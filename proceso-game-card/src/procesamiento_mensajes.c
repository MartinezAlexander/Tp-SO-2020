#include "procesamiento_mensajes.h"

//TODO cada llamado tiene que ser en un hilo nuevo

void ejecutar_new(t_new_pokemon* new_pokemon, int id) {
	//Obtengo el archivo del pokemon que necesito. Si no existe se crea.
	char* archivo_pokemon = obtener_pokemon(new_pokemon->pokemon->especie);

	abrir_archivo(archivo_pokemon);

	agregar_pokemon(archivo_pokemon, new_pokemon->pokemon->posicion, new_pokemon->cantidad);
	printf("[New] Se agrego la posicion al pokemon dado\n");

	sleep(tiempo_retardo_operacion);//Espero x segundos para simular el acceso al disco

	cerrar_archivo(archivo_pokemon);

	t_appeared_pokemon* appeared = appeared_pokemon_create(new_pokemon->pokemon->especie,
					new_pokemon->pokemon->posicion.posicionX,
					new_pokemon->pokemon->posicion.posicionY);
	t_mensaje* mensaje = mensaje_con_id_correlativo_create(appeared, APPEARED_POKEMON, id);
	enviar_mensaje_al_broker(mensaje);
}

void ejecutar_catch(t_catch_pokemon* catch_pokemon, int id) {
	//1. Verificar si el Pokemon existe dentro de nuestro Filesystem (Si no existe->se informa el error

	//PREGUNTAR EL SABADO: 'Se informa error' si no existe se loggea o se manda cuaght negativo??

	t_caught_pokemon* caught_respuesta;

	if (!existe_archivo_en(catch_pokemon->pokemon->especie, obtener_directorio_files())) {
		caught_respuesta = caught_pokemon_create(0);
	} else {
		char* archivo_pokemon = obtener_pokemon(catch_pokemon->pokemon->especie);
		//2. Verificar si se puede abrir el archivo (Si no->Se reintenta cada X seg)
		abrir_archivo(archivo_pokemon);

		//3. Verificar si las posiciones ya existen dentro del archivo (Si no->se informa error)
		int existe = existe_posicion(archivo_pokemon, catch_pokemon->pokemon->posicion);
		if(existe){
			//4. SI la cantidad = 1, se elimina la linea. Sino se decrementa la cantidad en 1
			decrementar_cantidad(archivo_pokemon, catch_pokemon->pokemon->posicion);
			printf("[Catch] Se decremento una cantidad para la posicion del pokemon\n");
		}

		//5. Esperar la cantidad de segundos definidos por archivo de configuracion
		sleep(tiempo_retardo_operacion);

		//6. Cerrar el archivo
		cerrar_archivo(archivo_pokemon);

		caught_respuesta = caught_pokemon_create(existe);
	}

	//7. Conectarse y enviar al broker el resultado (ID recibido, resultado)
	t_mensaje* mensaje = mensaje_con_id_correlativo_create(caught_respuesta,CAUGHT_POKEMON, id);
	enviar_mensaje_al_broker(mensaje);


}

void ejecutar_get(t_get_pokemon* get_pokemon, int id) {
	t_localized_pokemon* localized_respuesta;

	//1. Verificar si el Pokemon existe dentro de nuestro Filesystem
	if (!existe_archivo_en(get_pokemon->nombre, obtener_directorio_files())) {
		printf("[Get] El filesystem no tiene al pokemon dado\n");
		//TODO: (Si no existe->se informa el mensaje sin posiciones ni cantidades (???)
		//Me imagino (mas que nada por lo que dice al final) que si no hay pokemon
		//no hago nada. A lo sumo algun log pero no mando nada al broker.
		return;
	}

	char* archivo_pokemon = obtener_pokemon(get_pokemon->nombre);

	//2. Verificar si se puede abrir el archivo (Si no->Se reintenta cada X seg)
	abrir_archivo(archivo_pokemon);

	//3. Obtener todas las posiciones del Pokemon requerido
	t_list* pokemon_posiciones = obtener_posiciones_pokemon(archivo_pokemon);

	localized_respuesta = localized_pokemon_create(get_pokemon->nombre,pokemon_posiciones);
	printf("[Get] Se encontraron %d posiciones para el pokemon dado\n", list_size(pokemon_posiciones));

	//4. Esperar la cantidad de segundos definidos por archivo de configuracion
	sleep(tiempo_retardo_operacion);

	//5. Cerrar el archivo
	cerrar_archivo(archivo_pokemon);

	//6. Si se encontro al menos 1 posicion, conectarse y enviar al broker
	t_mensaje* mensaje = mensaje_con_id_correlativo_create((void*) localized_respuesta, LOCALIZED_POKEMON, id);
	enviar_mensaje_al_broker(mensaje);
}

void enviar_mensaje_al_broker(t_mensaje* mensaje){
	int socket = crear_conexion(ip_broker, puerto_broker);
	if (socket >= 0) {
		enviar_mensaje(mensaje, socket);
		recibir_id(socket);
		liberar_conexion(socket);
	} else {
		// loggeo error
		loggear_conexion(socket);
	}
}
