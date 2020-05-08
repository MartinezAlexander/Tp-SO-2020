#include "cola.h"


t_cola_mensajeria* cola_mensajeria_create(){
	t_cola_mensajeria* cola_mensajeria = malloc(sizeof(t_cola_mensajeria));
	cola_mensajeria->queue = queue_create();
	cola_mensajeria->suscriptores = list_create();
	return cola_mensajeria;
}
