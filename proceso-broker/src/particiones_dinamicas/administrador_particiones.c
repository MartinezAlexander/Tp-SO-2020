#include "administrador_particiones.h"

int es_fifo;

void iniciar_administrador_pd(){
	particiones = list_create();
	t_particion* primer_particion = particion_create(0,tamano_memoria,1);
	list_add(particiones,primer_particion);
	busquedas_fallidas = 0;
	if (string_equals_ignore_case(algoritmo_reemplazo, "FIFO")) {
		particiones_victimas = queue_create();
		es_fifo = 1;
	} else {
		es_fifo = 0;
		particiones_victimas_lru = list_create();
	}

	pthread_mutex_init(&mutex_cacheo,NULL);
}

void mostrar_estado_particiones(){
	for (int i = 0; i < list_size(particiones); i++) {
		t_particion* particion = list_get(particiones, i);
		puts("------------------------------------------------------");
		printf("Base %d Limite: %d - libre: %d tamanio: %d\n", particion->base,particion->limite,particion->libre, particion_tamanio(particion));
	}
}

int first_fit(t_mensaje* mensaje){

	int pude_cachear = 0;
	int tamanio_mensaje = mensaje_size(mensaje);
	int i;

	for(i = 0; i < list_size(particiones);i++){

		t_particion* particion = list_get(particiones,i);
		if(particion_esta_libre(particion) && particion_puede_guardarlo(particion,tamanio_mensaje)){
			t_particion* particion_libre = particion_ocuparla(particion,tamanio_mensaje);
			particion->cola = mensaje->codigo;
			particion->id = mensaje->id;
			particion->id_c = mensaje->id_correlativo;

			if(tamanio_mensaje < particion_tamanio(particion)){
				particion->tamanio_real = tamanio_mensaje;
			}

			if(particion_libre != NULL){
				//TODO Chequear si ubicacion_mejor_particion + 1 supera el list_size
				list_add_in_index(particiones,i+1,particion_libre);
			}

			memoria_cache_agregar_mensaje(mensaje,particion->base);
			loggear_mensaje_cacheado(mensaje_to_string(mensaje),particion->base);

			if(es_fifo){
				queue_push(particiones_victimas,particion);
			}else{
				list_add(particiones_victimas_lru,particion);
			}

			i = list_size(particiones);
			pude_cachear = 1;
		}
	}
	return pude_cachear;
}

int best_fit(t_mensaje* mensaje){
	int pude_cachear = 0;
	int tamanio_mensaje = mensaje_size(mensaje);

	t_particion* mejor_particion = NULL;
	int mejor_tamanio = tamano_memoria;
	int ubicacion_mejor_particion;

	int i;
	for(i = 0; i < list_size(particiones);i++){

		t_particion* particion = list_get(particiones,i);

		if(particion_esta_libre(particion) && particion_puede_guardarlo(particion,tamanio_mensaje)){

			if(particion_tamanio(particion) <= mejor_tamanio){
				mejor_particion = particion;
				mejor_tamanio = particion_tamanio(particion);
				ubicacion_mejor_particion = i;
			}
		}
	}

	if(mejor_particion != NULL){
		t_particion* particion_libre = particion_ocuparla(mejor_particion,tamanio_mensaje);

		mejor_particion->cola = mensaje->codigo;
		mejor_particion->id = mensaje->id;
		mejor_particion->id_c = mensaje->id_correlativo;

		if (tamanio_mensaje < particion_tamanio(mejor_particion)) {
			mejor_particion->tamanio_real = tamanio_mensaje;
		}

		if (particion_libre != NULL) {
			//TODO Chequear si ubicacion_mejor_particion + 1 supera el list_size
			list_add_in_index(particiones, ubicacion_mejor_particion + 1, particion_libre);
		}

		memoria_cache_agregar_mensaje(mensaje, mejor_particion->base);
		loggear_mensaje_cacheado(mensaje_to_string(mensaje),mejor_particion->base);

		if (es_fifo) {
			queue_push(particiones_victimas, mejor_particion);
		} else {
			list_add(particiones_victimas_lru, mejor_particion);
		}

		pude_cachear = 1;
	}

	return pude_cachear;
}

int es_hora_de_compactar(){
	return busquedas_fallidas == frecuencia_compactacion;
}

void fifo_eliminar(){
	t_particion* particion_victima = queue_pop(particiones_victimas);
	loggear_eliminacion_particion(particion_victima->base);
	particion_liberar(particion_victima);
	combinar_particiones_contiguas_a(particion_victima);
}

void actualizar_lru(t_particion* particion){
	int i;
	for(i = 0; i < list_size(particiones_victimas_lru); i++ ){
		t_particion* particion_aux = list_get(particiones_victimas_lru,i);
		if(particion == particion_aux){
			list_remove(particiones_victimas_lru,i);
			list_add(particiones_victimas_lru,particion);
			i = list_size(particiones_victimas_lru);
		}
	}
}

void lru_eliminar(){
	//TODO Una sola linea
	t_particion* particion_victima = list_get(particiones_victimas_lru,0);
	loggear_eliminacion_particion(particion_victima->base);
	list_remove(particiones_victimas_lru,0);
	particion_liberar(particion_victima);
	combinar_particiones_contiguas_a(particion_victima);
}

void combinar_particiones_contiguas_a(t_particion* particion_victima){
	int indice;
	for (int i = 0; i < list_size(particiones); i++) {
		t_particion* particion = list_get(particiones, i);
		if (particion_victima == particion) {
			indice = i;
			i = list_size(particiones);
		}
	}

	if ((indice + 1) < list_size(particiones)) {
		t_particion* particion_derecha = list_get(particiones, indice + 1);
		if (particion_esta_libre(particion_derecha)) {
			particion_combinar(particion_victima, particion_derecha);
			list_remove_and_destroy_element(particiones, indice + 1,
					(void*) particion_destroy);
		}
	}

	if ((indice - 1) >= 0) {
		t_particion* particion_izquierda = list_get(particiones, indice - 1);
		if (particion_esta_libre(particion_izquierda)) {
			particion_combinar(particion_izquierda, particion_victima);
			list_remove_and_destroy_element(particiones, indice,
					(void*) particion_destroy);
		}
	}
}

void compactar_particiones(){
	int base = 0;
	int i;

	for(i = 0; i < list_size(particiones); i++){
		t_particion* particion = list_get(particiones,i);
		if(particion_esta_libre(particion)){
			list_remove_and_destroy_element(particiones,i,(void*)particion_destroy);
			i--;
		}
	}

	for(i = 0; i < list_size(particiones); i++){
		t_particion* particion = list_get(particiones,i);
		int tamanio = particion_tamanio(particion);
		void* mensaje = memoria_cache_leer_stream(particion->base,tamanio);
		particion->base = base;
		particion->limite = base + tamanio;
		base = particion->limite;
		memoria_cache_agregar_stream(mensaje, particion->base,tamanio);

	}

	t_particion* particion_compactada = particion_create(base,tamano_memoria,1);
	list_add(particiones,particion_compactada);
	loggear_compactacion(particion_tamanio(particion_compactada),particion_compactada->base);
}

void procedimiento_para_almacenamiento_de_datos(t_mensaje* mensaje, int(*algoritmo)(t_mensaje* mensaje), void(*eliminar)(void)){
	int pudo_cachear = algoritmo(mensaje);
	while (!pudo_cachear) {
		//busquedas_fallidas++;
		if (es_hora_de_compactar()) {
			compactar_particiones();
			pudo_cachear = algoritmo(mensaje);
			busquedas_fallidas = 0;
		}
		if (!pudo_cachear) {
			eliminar();
			busquedas_fallidas++;
			pudo_cachear = algoritmo(mensaje);
		}
	}
}

void administrador_cachear_mensaje(t_mensaje* mensaje){

	//primer ajuste
	if(string_equals_ignore_case(algoritmo_particion_libre,"FF") &&  string_equals_ignore_case(algoritmo_reemplazo,"FIFO") ){
		pthread_mutex_lock(&mutex_cacheo);
		procedimiento_para_almacenamiento_de_datos(mensaje,first_fit, fifo_eliminar);
		pthread_mutex_unlock(&mutex_cacheo);
	}
	if (string_equals_ignore_case(algoritmo_particion_libre, "FF") &&  string_equals_ignore_case(algoritmo_reemplazo,"LRU")) {
		pthread_mutex_lock(&mutex_cacheo);
		procedimiento_para_almacenamiento_de_datos(mensaje, first_fit, lru_eliminar);
		pthread_mutex_unlock(&mutex_cacheo);
	}
	if (string_equals_ignore_case(algoritmo_particion_libre, "BF") && string_equals_ignore_case(algoritmo_reemplazo, "FIFO")) {
		pthread_mutex_lock(&mutex_cacheo);
		procedimiento_para_almacenamiento_de_datos(mensaje, best_fit, fifo_eliminar);
		pthread_mutex_unlock(&mutex_cacheo);
	}
	if (string_equals_ignore_case(algoritmo_particion_libre, "BF") && string_equals_ignore_case(algoritmo_reemplazo, "LRU")) {
		pthread_mutex_lock(&mutex_cacheo);
		procedimiento_para_almacenamiento_de_datos(mensaje, best_fit, lru_eliminar);
		pthread_mutex_unlock(&mutex_cacheo);
	}
}

t_list* obtener_mensajes_cacheados_por_cola_pd(op_code cola){
	t_list* mensajes = list_create();
	int hay_mensajes_de_esa_cola = 0;

	for (int i = 0; i < list_size(particiones); i++) {
		t_particion* particion = list_get(particiones, i);
		if (!particion_esta_libre(particion)) {
			if (particion->cola == cola) {
				t_mensaje* mensaje_c = memoria_cache_leer_mensaje(particion->base, particion_tamanio(particion), cola);
				mensaje_c->codigo = particion->cola;
				mensaje_c->id = particion->id;
				mensaje_c->id_correlativo = particion->id_c;
				list_add(mensajes,mensaje_c);
				if(!es_fifo){
					actualizar_lru(particion);
				}
				hay_mensajes_de_esa_cola = 1;
			}
		}
	}

	if(!hay_mensajes_de_esa_cola){
		list_destroy(mensajes);
		mensajes = NULL;
	}

	return mensajes;
}

char* particion_to_string(t_particion* particion) {
	void* base = memoria_cache->cache + particion->base;
	void* limite = base + particion->limite;
	int size = particion->limite - particion->base;
	char* libre = "[L]";
	char* ocupado = "[X]";
	char* string;
	if (particion_esta_libre(particion)) {
		string = string_from_format("%p - %p. %s Size:%d b", base, limite,
				libre, size);
	} else {
		char* cola = op_code_to_string(particion->cola);
		int id = particion->id;
		if (string_equals_ignore_case(algoritmo_reemplazo, "LRU")) {
			int lru;
			for (int i = 0; i < list_size(particiones_victimas_lru); i++) {
				t_particion* victima = list_get(particiones_victimas_lru, i);
				if (victima == particion) {
					lru = i;
					i = list_size(particiones_victimas_lru);
				}
			}
			string = string_from_format(
					"%p - %p. %s Size:%d b LRU:%d COLA:%s ID:%d", base,
					limite, ocupado, size, lru, cola, id);
		} else {
			string = string_from_format(
					"%p - %p. %s Size:%d b LRU:NO COLA:%s ID:%d", base,
					limite, ocupado, size, cola, id);
		}
	}
	return string;
}

void finalizar_particiones_dinamicas(){
	if(string_equals_ignore_case(algoritmo_reemplazo,"FIFO")){
		queue_destroy(particiones_victimas);
	}else{
		list_destroy(particiones_victimas_lru);
	}
	list_destroy_and_destroy_elements(particiones,(void*)particion_destroy);
}
