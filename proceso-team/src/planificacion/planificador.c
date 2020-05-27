#include "planificador.h"

tipo_planificacion obtener_algoritmo(char* algoritmo){
	if(strcmp(algoritmo,"FIFO") == 0) return FIFO;
	else if(strcmp(algoritmo,"RR") == 0) return RR;
	else if(strcmp(algoritmo,"SJF_CD") == 0) return SJF_CD;
	else if(strcmp(algoritmo,"SJF_SD") == 0) return SJF_SD;
	else{
		printf("\n --Algoritmo de planificacion invalido--\n\n");
		exit(2);
	}
}

t_planificador* planificador_create(char* algoritmo, uint32_t quantum, uint32_t estimacion_inicial)
{
	t_planificador* planificador = malloc(sizeof(t_planificador));
	planificador->algoritmo_planificacion = obtener_algoritmo(algoritmo);
	planificador->quantum = quantum;
	planificador->estimacion_inicial = estimacion_inicial;
	planificador->cola = queue_create();
	planificador->entrenadorEnExec = NULL;
	sem_init(&(planificador->semaforo), 0, 1);
	planificador->quantum_actual = 0;
    return planificador;
}
int puedo_ejecutar()
{
    return !list_any_satisfy(entrenadores, (void*)entrenador_en_ejecucion);
}

