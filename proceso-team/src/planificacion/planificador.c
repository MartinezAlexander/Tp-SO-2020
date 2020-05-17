#include "planificador.h"

tipo_planificacion obtener_algoritmo(char* algoritmo){
	if(strcmp(algoritmo,"FIFO")) return FIFO;
	else if(strcmp(algoritmo,"RR")) return RR;
	else if(strcmp(algoritmo,"SJF_CD")) return SJF_CD;
	else if(strcmp(algoritmo,"SJF_SD")) return SJF_SD;
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
    return planificador;
}
