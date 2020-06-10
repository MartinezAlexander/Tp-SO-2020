#include "administrador_buddy_system.h"


void iniciar_buddy_system(){
	double exp = log2((double)tamano_memoria);
	int exp_int = (int)exp;

	if(exp - exp_int == 0){
		//t_buddy* buddy = buddy_create(exp_int,0);

		//TODO cosas

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

void bs_cachear_mensaje(t_mensaje* mensaje){
	//TODO esto
}

