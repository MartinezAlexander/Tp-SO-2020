#include "planificacion_fifo.h"

void planificar_fifo(){
	if(puedo_ejecutar()){
		t_entrenador* entrenador = queue_pop(planificador->cola);
		planificador->entrenadorEnExec = entrenador;
		entrenador->estado = EXEC;
		sem_post(&(entrenador->semaforo));
		}else{
			sem_post(&(planificador->entrenadorEnExec->semaforo));
		}
}
