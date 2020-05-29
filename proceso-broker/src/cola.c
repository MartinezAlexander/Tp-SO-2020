#include "cola.h"

t_cola_mensajeria* cola_mensajeria_create(){
	t_cola_mensajeria* cola_mensajeria = malloc(sizeof(t_cola_mensajeria));
	cola_mensajeria->queue = queue_create();
	cola_mensajeria->suscriptores = list_create();
	return cola_mensajeria;
}

t_cola_mensajeria* cola_mensajeria_obtener(op_code codigo){
	t_cola_mensajeria* cola;
	switch(codigo){
		case NEW_POKEMON:
			cola = cola_mensajeria_new;
			break;
		case LOCALIZED_POKEMON:
			cola = cola_mensajeria_localized;
			break;
		case GET_POKEMON:
			cola = cola_mensajeria_get;
			break;
		case APPEARED_POKEMON:
			cola = cola_mensajeria_appeared;
			break;
		case CATCH_POKEMON:
			cola = cola_mensajeria_catch;
			break;
		case CAUGHT_POKEMON:
			cola = cola_mensajeria_caught;
			break;
		default:
			break;
	}
	return cola;
}