#include "planificacion.h"

t_planificador* planificador_create(char* algoritmo, uint32_t quantum, uint32_t estimacion_inicial)
{
	t_planificador* planificador = malloc(sizeof(t_planificador));
	planificador->algoritmo_planificacion = algoritmo;
	planificador->quantum = quantum;
	planificador->estimacion_inicial = estimacion_inicial;
	planificador->cola = queue_create();
    return planificador;
}

void planificar(t_posicion posicion_pokemon){
	t_list* entrenadores_disponibles = list_filter(entrenadores, entrenador_disponible);
	t_entrenador *entrenador_cercano = obtener_entrenador_cercano(entrenadores_disponibles, posicion_pokemon);
	encolar(entrenador_cercano);
	algoritmo_planificar();
}


// Hay distintos tipos de bloqueado ??
int entrenador_disponible(t_entrenador *entrenador)
{
	return entrenador->estado == NEW || entrenador->estado == BLOCKED;
}

void encolar(t_entrenador *entrenador)
{
    entrenador->estado = READY;
	queue_push(planificador->cola, entrenador);
}

int puedo_ejecutar()
{
    return !list_any_satisfy(entrenadores, checkear_exec_entrenador);
}

//TODO
t_entrenador *obtener_entrenador_cercano(t_list *entrenadores, t_posicion *posicion_pokemon)
{
	return list_get(entrenadores, 1);
}

//TODO En progreso
/*void algoritmo_planificar()
{
	switch(planificador->algoritmo_planificacion){
		case "FIFO":

		break;
	}
}*/
