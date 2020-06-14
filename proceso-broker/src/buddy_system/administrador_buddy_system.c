#include "administrador_buddy_system.h"


void iniciar_buddy_system(){
	double exp = log2((double)tamano_memoria);
	int exp_int = (int)exp;

	if(exp - exp_int == 0){
		buddy_principal = buddy_create(exp_int,0);
	}
	else{
		puts("No se puede implementar un BS de un tamano que no sea potencia de 2");
		exit(1);
	}
}

t_list* obtener_mensajes_cacheados_por_cola_bs(op_code cola){
	//TODO esto
	return 0;
}

void fifo();
void lru();

int bs_cachear_mensaje(t_mensaje* mensaje){
	int pude_guardar = 0;
	t_list* hojas = list_create();
	recolectar_hojas_libres(buddy_principal,hojas);
	list_sort(hojas, menor_a_mayor);

	for(int i = 0; i < list_size(hojas); i++){
		t_buddy* buddy = list_get(hojas,i);
		if (!pude_guardar){
			if(tamanio_ideal_para_el_buddy(mensaje,buddy)){
				buddy_ocupar(buddy,mensaje_size(mensaje));
				memoria_cache_agregar_mensaje(mensaje,buddy->base);
				pude_guardar = 1;
			}
		}
	}
	list_destroy(hojas);
	return pude_guardar;
}

int splittear(t_mensaje* mensaje){
	int pudo_cachear = 0;
	t_list* hojas = list_create();
	recolectar_hojas_libres(buddy_principal,hojas);
	list_sort(hojas, menor_a_mayor);

	for(int i = 0; i < list_size(hojas); i++){
		t_buddy* buddy = list_get(hojas,i);

		if(mensaje_size(mensaje) <= buddy_get_real_size(buddy)){
			buddy_split(buddy);
			if(tamanio_ideal_para_el_buddy(mensaje, buddy->izq)){
				pudo_cachear = bs_cachear_mensaje(mensaje);
			}else{
				i = list_size(hojas);
			}
		}
	}
	list_destroy(hojas);

	if (!pudo_cachear){
		splittear(mensaje);
	}
	return 1;//TODO
}

void procedimiento_para_almacenamiento_de_datos(t_mensaje* mensaje, int(*algoritmo)(t_mensaje* mensaje), void(*eliminar)(void)){
	int pudo_cachear = algoritmo(mensaje);
	if (!pudo_cachear){
		pudo_cachear = splittear(mensaje);
	}

	while (!pudo_cachear) {
		eliminar();
		pudo_cachear = algoritmo(mensaje);
	}
}

void cachear_mensaje_bs(t_mensaje* mensaje){
	if(string_equals_ignore_case(algoritmo_reemplazo,"FIFO")){
		//mutex
		procedimiento_para_almacenamiento_de_datos(mensaje,bs_cachear_mensaje,fifo);
		//mutex
	}else{
		//mutex
		procedimiento_para_almacenamiento_de_datos(mensaje,bs_cachear_mensaje,lru);
		//mutex
	}
}





/*
int es_hora_de_compactar(int tamanio_a_guardar){
	t_list* hojas = list_create();
	recolectar_hojas_libres(buddy_principal,hojas);
	int espacio_libre = 0;
	for(int i = 0; i < list_size(hojas); i++){
		t_buddy* buddy = list_get(hojas,i);
		espacio_libre += buddy_get_real_size(buddy);
	}
	return tamanio_a_guardar<=espacio_libre?1:0;
}
*/
