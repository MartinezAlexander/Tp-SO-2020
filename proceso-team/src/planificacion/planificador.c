#include "planificador.h"

tipo_planificacion obtener_algoritmo(char* algoritmo){
	if(strcmp(algoritmo,"FIFO") == 0) return FIFO;
	else if(strcmp(algoritmo,"RR") == 0) return RR;
	else if(strcmp(algoritmo,"SJF-CD") == 0) return SJF_CD;
	else if(strcmp(algoritmo,"SJF-SD") == 0) return SJF_SD;
	else{
		printf("\n --Algoritmo de planificacion invalido--\n\n");
		exit(2);
	}
}

t_planificador* planificador_create(char* algoritmo, uint32_t quantum, uint32_t estimacion_inicial, double alpha)
{
	t_planificador* planificador = malloc(sizeof(t_planificador));
	planificador->algoritmo_planificacion = obtener_algoritmo(algoritmo);
	planificador->quantum = quantum;
	planificador->estimacion_inicial = estimacion_inicial;
	planificador->alpha = alpha;
	planificador->cola = queue_create();
	planificador->entrenador_en_exec = NULL;
	sem_init(&(planificador->semaforo), 0, 1);
	planificador->quantum_actual = 0;
    return planificador;
}

int hay_alguien_en_ejecucion(){
	if(planificador->entrenador_en_exec == NULL) return 0;
	else return entrenador_en_ejecucion(planificador->entrenador_en_exec);
}

void entrar_a_ejecucion(t_entrenador* entrenador){
	planificador->entrenador_en_exec = entrenador;
	entrenador->estado = EXEC;
	//Estadisticas
	cambios_de_contexto++;
}

void sacar_de_ejecucion(){
	//Estadisticas
	if(planificador->entrenador_en_exec != NULL){
		cambios_de_contexto++;
	}
	planificador->entrenador_en_exec = NULL;
}

void encolar(t_entrenador* entrenador)
{
	printf("Encolado entrenador en [%d , %d]\n", entrenador->posicion.posicionX,  entrenador->posicion.posicionY);
    entrenador->estado = READY;
	queue_push(planificador->cola, entrenador);
}

