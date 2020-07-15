#include "procesamiento_mensajes.h"

void procesar_mensaje(t_mensaje* mensaje_recibido, void* funcion_ejecutar) {
	pthread_t hilo_procesamiento;
	//TODO ver si funciona puntero al hilo data race condition
	//TODO Thread #7 was created [PID: 8835]
	pthread_create(&hilo_procesamiento, NULL, (void*) funcion_ejecutar, (void*) mensaje_recibido);
	pthread_detach(hilo_procesamiento);
	cantidad_hilos++;
	//printf("[Procesamiento] Creado nuevo hilo para atender mensaje\n");
}

void ejecutar_new(t_mensaje* mensaje_recibido) {
	int numero_hilo = cantidad_hilos;

	t_new_pokemon* new_pokemon = (t_new_pokemon*) mensaje_recibido->mensaje;
	int id = mensaje_recibido->id;

	puts("------------------------------------------------------");
	printf("[NEW] Soy el hilo %d para procesar especie %s\n",numero_hilo,new_pokemon->pokemon->especie);

	//Obtengo el archivo del pokemon que necesito. Si no existe se crea.
	pthread_mutex_lock(&mutex_obtener_pokemon);
	//char* archivo_pokemon = obtener_pokemon(new_pokemon->pokemon->especie);
	file_pokemon* poke_file = file_pokemon_obtener(new_pokemon->pokemon->especie);
	pthread_mutex_unlock(&mutex_obtener_pokemon);

	//abrir_archivo(poke_file,numero_hilo);
	abrir_archivo_dos(poke_file,numero_hilo);
	printf(" %s\n",new_pokemon->pokemon->especie);

	//agregar_pokemon(poke_file, new_pokemon->pokemon->posicion, new_pokemon->cantidad);
	agregar_pokemon_dos(poke_file, new_pokemon->pokemon->posicion, new_pokemon->cantidad);

	printf("[NEW] Soy el hilo %d y agrego la posicion a la especie %s dado\n",numero_hilo,new_pokemon->pokemon->especie);

	sleep(tiempo_retardo_operacion); //Espero x segundos para simular el acceso al disco

	//cerrar_archivo(poke_file);
	cerrar_archivo_dos(poke_file);
	printf("[NEW] Soy el hilo %d y cerre el archivo de especie %s\n",numero_hilo,new_pokemon->pokemon->especie);

	//free(archivo_pokemon);

	t_appeared_pokemon* appeared = appeared_pokemon_create(
			new_pokemon->pokemon->especie,
			new_pokemon->pokemon->posicion.posicionX,
			new_pokemon->pokemon->posicion.posicionY);

	t_mensaje* mensaje = mensaje_con_id_correlativo_create(appeared, APPEARED_POKEMON, id);
	enviar_mensaje_al_broker(mensaje);
	mensaje_destroy(mensaje);
	mensaje_destroy(mensaje_recibido);

}

void ejecutar_catch(t_mensaje* mensaje_recibido) {
	int numero_hilo = cantidad_hilos;
	puts("------------------------------------------------------");
	printf("[Procesamiento] Hilo %d para procesar mensaje CATCH\n",numero_hilo);

	t_catch_pokemon* catch_pokemon = (t_catch_pokemon*) mensaje_recibido->mensaje;
	int id = mensaje_recibido->id;

	//1. Verificar si el Pokemon existe dentro de nuestro Filesystem (Si no existe->se informa el error
	t_caught_pokemon* caught_respuesta;

	if (!existe_archivo_en(catch_pokemon->pokemon->especie,obtener_directorio_files())) {
		caught_respuesta = caught_pokemon_create(0);
		loggear_no_existe_pokemon(catch_pokemon->pokemon->especie);
	} else {
		//char* archivo_pokemon = obtener_pokemon(catch_pokemon->pokemon->especie);

		pthread_mutex_lock(&mutex_obtener_pokemon);
		file_pokemon* poke_file = file_pokemon_obtener(catch_pokemon->pokemon->especie);
		pthread_mutex_unlock(&mutex_obtener_pokemon);

		//2. Verificar si se puede abrir el archivo (Si no->Se reintenta cada X seg)

		//abrir_archivo(poke_file,numero_hilo);
		abrir_archivo_dos(poke_file,numero_hilo);

		//3. Verificar si las posiciones ya existen dentro del archivo (Si no->se informa error)

		int existe = decrementar_cantidad(poke_file, catch_pokemon->pokemon->posicion);
		//int existe = decrementar_cantidad_dos(poke_file,catch_pokemon->pokemon->posicion);

		//5. Esperar la cantidad de segundos definidos por archivo de configuracion
		sleep(tiempo_retardo_operacion);

		//6. Cerrar el archivo

		//cerrar_archivo(poke_file);
		cerrar_archivo_dos(poke_file);

		caught_respuesta = caught_pokemon_create(existe);
	}

	//7. Conectarse y enviar al broker el resultado (ID recibido, resultado)

	t_mensaje* mensaje = mensaje_con_id_correlativo_create(caught_respuesta,CAUGHT_POKEMON, id);
	enviar_mensaje_al_broker(mensaje);
	mensaje_destroy(mensaje);
	mensaje_destroy(mensaje_recibido);

	printf("[Procesamiento] Cerrando hilo %d de procesamiento CATCH\n",numero_hilo);
}

void ejecutar_get(t_mensaje* mensaje_recibido) {
	int numero_hilo = cantidad_hilos;
	puts("------------------------------------------------------");
	printf("[Procesamiento] Hilo %d para procesar mensaje GET\n",numero_hilo);

	t_get_pokemon* get_pokemon = (t_get_pokemon*) mensaje_recibido->mensaje;
	int id = mensaje_recibido->id;

	t_localized_pokemon* localized_respuesta;

	//1. Verificar si el Pokemon existe dentro de nuestro Filesystem
	if (!existe_archivo_en(get_pokemon->nombre, obtener_directorio_files())) {
		printf("[Get] El filesystem no tiene al pokemon dado\n");

		loggear_no_existe_pokemon(get_pokemon->nombre);
		return;
	}

	//char* archivo_pokemon = obtener_pokemon(get_pokemon->nombre);

	pthread_mutex_lock(&mutex_obtener_pokemon);
	file_pokemon* poke_file = file_pokemon_obtener(get_pokemon->nombre);
	pthread_mutex_unlock(&mutex_obtener_pokemon);

	//2. Verificar si se puede abrir el archivo (Si no->Se reintenta cada X seg)
	//abrir_archivo(poke_file,numero_hilo);
	abrir_archivo_dos(poke_file,numero_hilo);

	//3. Obtener todas las posiciones del Pokemon requerido

	//t_list* pokemon_posiciones = obtener_posiciones_pokemon(poke_file);
	t_list* pokemon_posiciones = obtener_posiciones_pokemon_dos(poke_file);

	localized_respuesta = localized_pokemon_create(get_pokemon->nombre,pokemon_posiciones);
	printf("[Get] Se encontraron %d posiciones para el pokemon dado\n",list_size(pokemon_posiciones));

	//4. Esperar la cantidad de segundos definidos por archivo de configuracion
	sleep(tiempo_retardo_operacion);

	//5. Cerrar el archivo

	//cerrar_archivo(poke_file);
	//free(archivo_pokemon);
	cerrar_archivo_dos(poke_file);

	//6. Si se encontro al menos 1 posicion, conectarse y enviar al broker

	if(list_size(pokemon_posiciones)>=1){
	t_mensaje* mensaje = mensaje_con_id_correlativo_create((void*) localized_respuesta, LOCALIZED_POKEMON, id);
	//67 (16 direct, 51 indirect) bytes in 1 blocks are definitely lost
	enviar_mensaje_al_broker(mensaje);
	mensaje_destroy(mensaje);
	mensaje_destroy(mensaje_recibido);
	printf("[Procesamiento] Cerrando hilo %d de procesamiento GET\n",numero_hilo);
	}

}

void enviar_mensaje_al_broker(t_mensaje* mensaje) {
	int socket = crear_conexion(ip_broker, puerto_broker);
	if (socket >= 0) {
		enviar_mensaje(mensaje, socket);
		recibir_id(socket);
		liberar_conexion(socket);
	} else {
		loggear_error_conexion(socket);
	}
}
