#include "planificacion_fifo.h"

void comenzar_ciclo_fifo(t_entrenador* entrenador){
	//wait del samaforo del entrenador
	sem_wait(&(entrenador->semaforo));
}

void finalizar_ciclo_fifo(t_entrenador* entrenador){
	//Termino de ejecutar?
	if(entrenador->estado == BLOCKED){
		//Si: Pasar a ejecucion el primero en la cola
		ejecutar_proximo_fifo();
	}else{
		//No: libero el semaforo del entrenador para que siga su ejecucion
		sem_post(&(entrenador->semaforo));
	}
}

//A definir para cada planificador
void ejecutar_proximo_fifo(){
	//Valido que la cola tenga elementos
	if(!queue_is_empty(planificador->cola)){
		//Desencolo el primer entrenador
		t_entrenador* entrenador = queue_pop(planificador->cola);

		printf("Entrenador en [%d , %d] pasa a ejecucion\n", entrenador->posicion.posicionX,  entrenador->posicion.posicionY);

		//signal de su semaforo para que arranque
		sem_post(&(entrenador->semaforo));
		entrenador->estado = EXEC;
	}
}
