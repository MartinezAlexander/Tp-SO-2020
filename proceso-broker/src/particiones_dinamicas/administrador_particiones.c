#include "administrador_particiones.h"

int es_fifo;

void iniciar_administrador(){
	particiones = list_create();
	t_particion* primer_particion = particion_create(0,tamano_memoria,1);
	list_add(particiones,primer_particion);
	busquedas_fallidas = 0;
	if (string_equals_ignore_case(algoritmo_reemplazo, "FIFO")) {
		particiones_victimas = queue_create();
		es_fifo = 1;
	} else {
		es_fifo = 0;
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

			if(particion_libre != NULL){
				list_add_in_index(particiones,i+1,particion_libre);
			}

			memoria_cache_agregar_mensaje(mensaje,particion->base);

			if(es_fifo){
				queue_push(particiones_victimas,particion);
			}

			i = list_size(particiones);
			pude_cachear = 1;
		}
	}
	return pude_cachear;
}

int best_fit(t_mensaje* mensaje){
	//TODO falta best_fit
	return 0;
}

int es_hora_de_compactar(){
	return busquedas_fallidas == frecuencia_compactacion;
}

void fifo_eliminar(){
	t_particion* particion_victima = queue_pop(particiones_victimas);
	particion_liberar(particion_victima);

	int indice;
	for(int i = 0; i < list_size(particiones); i++){
		t_particion* particion = list_get(particiones,i);
		if(particion_victima == particion){
			indice = i;
			i = list_size(particiones);
		}
	}

	if((indice + 1) < list_size(particiones)){
		t_particion* particion_derecha = list_get(particiones,indice + 1);
		if(particion_esta_libre(particion_derecha)){
			particion_combinar(particion_victima,particion_derecha);
			list_remove_and_destroy_element(particiones,indice + 1, (void*)particion_destroy);
		}
	}

	if ((indice - 1) >= 0) {
		t_particion* particion_izquierda = list_get(particiones,indice - 1);
		if (particion_esta_libre(particion_izquierda)) {
			particion_combinar(particion_izquierda, particion_victima);
			list_remove_and_destroy_element(particiones,indice, (void*)particion_destroy);
		}
	}
}

void lru_eliminar(){
	// TODO falta eliminar particiones por lru
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

}

void procedimiento_para_almacenamiento_de_datos(t_mensaje* mensaje, int(*algoritmo)(t_mensaje* mensaje), void(*eliminar)(void)){
	int pudo_cachear = algoritmo(mensaje);
	while (!pudo_cachear) {
		busquedas_fallidas++;
		if (es_hora_de_compactar()) {
			compactar_particiones();
			pudo_cachear = algoritmo(mensaje);
			busquedas_fallidas = 0;
		}
		if (!pudo_cachear) {
			eliminar();
			pudo_cachear = algoritmo(mensaje);
		}
	}
}

void administrador_cachear_mensaje(t_mensaje* mensaje){

	//primer ajuste
	if(string_equals_ignore_case(algoritmo_particion_libre,"FF") &&  string_equals_ignore_case(algoritmo_reemplazo,"FIFO") ){
		procedimiento_para_almacenamiento_de_datos(mensaje,first_fit, fifo_eliminar);
	}
	if (string_equals_ignore_case(algoritmo_particion_libre, "FF") &&  string_equals_ignore_case(algoritmo_reemplazo,"LRU")) {
		procedimiento_para_almacenamiento_de_datos(mensaje, first_fit, lru_eliminar);
	}
	if (string_equals_ignore_case(algoritmo_particion_libre, "BF") && string_equals_ignore_case(algoritmo_reemplazo, "FIFO")) {
		procedimiento_para_almacenamiento_de_datos(mensaje, best_fit, fifo_eliminar);
	}
	if (string_equals_ignore_case(algoritmo_particion_libre, "BF") && string_equals_ignore_case(algoritmo_reemplazo, "LRU")) {
		procedimiento_para_almacenamiento_de_datos(mensaje, best_fit, lru_eliminar);
	}
}
t_list* obtener_mensajes_cacheados_por_cola(op_code cola){
	t_list* mensajes = list_create();
	int hay_mensajes_de_esa_cola = 0;

	for (int i = 0; i < list_size(particiones); i++) {
		t_particion* particion = list_get(particiones, i);
		if (!particion_esta_libre(particion)) {
			if (memoria_cache_es_un_mensaje_tipo(particion->base,particion_tamanio(particion), cola)) {
				particion->lru = time(NULL);
				t_mensaje* mensaje_c = memoria_cache_leer_mensaje(particion->base, particion_tamanio(particion));
				list_add(mensajes,mensaje_c);
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

void memoria_cache_enviar_mensajes_cacheados(para_envio_mensaje_cacheados* parametros){
	t_list* mensajes = obtener_mensajes_cacheados_por_cola(parametros->cola);

	if(mensajes != NULL){
		int i;
		for (i = 0; i < list_size(mensajes); i++) {
			t_mensaje* mensaje = (t_mensaje*) list_get(mensajes, i);
			int resultado_envio = enviar_mensaje(mensaje,parametros->suscriptor->socket);

			if (resultado_envio > 0) {
				//TODO pasar a log personal
				loggear_envio_mensaje(mensaje_to_string(mensaje));

				//TODO verificar retorno de ack
				recibir_ACK(parametros->suscriptor->socket);
				loggear_recepcion_ACK(suscriptor_to_string(parametros->suscriptor));
			} else {
				log_personal_error_envio_a_suscriptor(suscriptor_to_string(parametros->suscriptor));
				i = list_size(mensajes);
			}
		}
		list_destroy_and_destroy_elements(mensajes,(void*)mensaje_destroy);
	}

	parametros_destroy(parametros);
}

para_envio_mensaje_cacheados* parametros_create(t_suscriptor* suscriptor, op_code cola, t_memoria_cache* memoria){
	para_envio_mensaje_cacheados* parametros = malloc(sizeof(para_envio_mensaje_cacheados));
	parametros->suscriptor = suscriptor;
	parametros->cola = cola;
	parametros->memoria = memoria;
	return parametros;
}

void parametros_destroy(para_envio_mensaje_cacheados* parametros){
	free(parametros);
}

