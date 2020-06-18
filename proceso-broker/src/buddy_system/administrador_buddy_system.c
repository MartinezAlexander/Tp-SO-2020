#include "administrador_buddy_system.h"


void iniciar_buddy_system(){
	double exp = log2((double)tamano_memoria);
	int exp_int = (int)exp;

	if(exp - exp_int == 0){
		buddy_principal = buddy_create(exp_int,0);
		if(string_equals_ignore_case(algoritmo_reemplazo,"LRU")){
			buddies_victimas_lru = list_create();
		}else{
			buddies_victimas_fifo = queue_create();
		}
		pthread_mutex_init(&mutex_bs,NULL);
	}
	else{
		puts("No se puede implementar un BS de un tamano que no sea potencia de 2");
		exit(1);
	}
}

t_list* obtener_mensajes_cacheados_por_cola_bs(op_code cola){
	t_list* mensajes = list_create();
	t_list* hojas_ocupadas = list_create();

	recolectar_hojas_ocupadas(buddy_principal,hojas_ocupadas);

	for(int i = 0; i < list_size(hojas_ocupadas); i++){
		t_buddy* buddy = list_get(hojas_ocupadas,i);
		if(buddy->cola == cola){
			t_mensaje* mensaje = memoria_cache_leer_mensaje(buddy->base,buddy->tamanio_mensaje,cola);
			mensaje->codigo = cola;
			mensaje->id = buddy->id;
			mensaje->id_correlativo = buddy->id_c;
			list_add(mensajes,mensaje);
			if(string_equals_ignore_case(algoritmo_reemplazo,"LRU")){
				lru_actualizar(buddy);
			}
		}
	}
	list_destroy(hojas_ocupadas);

	return mensajes;
}

void fifo(){
	t_buddy* buddy_victima = queue_pop(buddies_victimas_fifo);
	buddy_mostrar(buddy_victima);
	buddy_liberar(buddy_victima);
}

void lru_actualizar(t_buddy* buddy){
	int i;
	for(i = 0; i < list_size(buddies_victimas_lru); i++ ){
		t_buddy* buddy_aux = list_get(buddies_victimas_lru,i);
		if(buddy == buddy_aux){
			list_remove(buddies_victimas_lru,i);
			list_add(buddies_victimas_lru,buddy);
			i = list_size(buddies_victimas_lru);
		}
	}
}

void lru(){
	t_buddy* buddy_victima = list_get(buddies_victimas_lru,0);
	list_remove(buddies_victimas_lru,0);
	buddy_liberar(buddy_victima);
}

int ff(t_mensaje* mensaje){
	int pude_guardar = 0;
	t_list* hojas = list_create();
	recolectar_hojas_libres(buddy_principal,hojas);

	t_buddy* seleccionado = NULL;

	for(int i = 0; i < list_size(hojas); i++){
		t_buddy* buddy = list_get(hojas,i);
		if(mensaje_entra_en_buddy(mensaje,buddy)){
			seleccionado = buddy;
			i = list_size(hojas);
		}
	}

	if(seleccionado != NULL){
		pude_guardar = 1;
		while((buddy_get_real_size(seleccionado)/2 >= tamano_minimo_particion) && !tamanio_ideal_para_el_buddy(mensaje,seleccionado)){
			buddy_split(seleccionado);
			seleccionado = (t_buddy*)seleccionado->izq;
		}
		buddy_ocupar(seleccionado,mensaje);
		memoria_cache_agregar_mensaje(mensaje,seleccionado->base);
		loggear_mensaje_cacheado(mensaje_to_string(mensaje),seleccionado->base);
		//mensaje_destroy(mensaje);
	}

	list_destroy(hojas);
	return pude_guardar;
}

int bf(t_mensaje* mensaje){
	int pude_guardar = 0;
	t_list* hojas = list_create();
	recolectar_hojas_libres(buddy_principal,hojas);
	list_sort(hojas, menor_a_mayor);

	t_buddy* seleccionado = NULL;

	for(int i = 0; i < list_size(hojas); i++){
		t_buddy* buddy = list_get(hojas,i);
		if(mensaje_entra_en_buddy(mensaje,buddy)){
			seleccionado = buddy;
			i = list_size(hojas);
		}
	}

	if(seleccionado != NULL){
		pude_guardar = 1;
		while((buddy_get_real_size(seleccionado)/2 >= tamano_minimo_particion) && !tamanio_ideal_para_el_buddy(mensaje,seleccionado)){
			buddy_split(seleccionado);
			seleccionado = (t_buddy*)seleccionado->izq;
		}
		buddy_ocupar(seleccionado,mensaje);
		memoria_cache_agregar_mensaje(mensaje,seleccionado->base);
		loggear_mensaje_cacheado(mensaje_to_string(mensaje),seleccionado->base);
		//mensaje_destroy(mensaje);
	}

	list_destroy(hojas);
	return pude_guardar;
}

void proc_para_almacenamiento_de_datos(t_mensaje* mensaje, int(*algoritmo)(t_mensaje* mensaje), void(*eliminar)(void)){
	int pudo_cachear = algoritmo(mensaje);
	while (!pudo_cachear) {
		eliminar();
		pudo_cachear = algoritmo(mensaje);
	}
}

void cachear_mensaje_bs(t_mensaje* mensaje){

	//primer ajuste
	if(string_equals_ignore_case(algoritmo_particion_libre,"FF") &&  string_equals_ignore_case(algoritmo_reemplazo,"FIFO") ){
		pthread_mutex_lock(&mutex_bs);
		proc_para_almacenamiento_de_datos(mensaje,ff, fifo);
		pthread_mutex_unlock(&mutex_bs);
	}
	if (string_equals_ignore_case(algoritmo_particion_libre, "FF") &&  string_equals_ignore_case(algoritmo_reemplazo,"LRU")) {
		pthread_mutex_lock(&mutex_bs);
		proc_para_almacenamiento_de_datos(mensaje, ff, lru);
		pthread_mutex_unlock(&mutex_bs);
	}
	if (string_equals_ignore_case(algoritmo_particion_libre, "BF") && string_equals_ignore_case(algoritmo_reemplazo, "FIFO")) {
		pthread_mutex_lock(&mutex_bs);
		proc_para_almacenamiento_de_datos(mensaje, bf, fifo);
		pthread_mutex_unlock(&mutex_bs);
	}
	if (string_equals_ignore_case(algoritmo_particion_libre, "BF") && string_equals_ignore_case(algoritmo_reemplazo, "LRU")) {
		pthread_mutex_lock(&mutex_bs);
		proc_para_almacenamiento_de_datos(mensaje, bf, lru);
		pthread_mutex_unlock(&mutex_bs);
	}
}

