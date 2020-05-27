#include "planificacion_rr.h"

//TODO Re-hacer mas declarativo
void planificar_rr(){
	//Checkeo si termino el quantum
	if(planificador->quantum_actual == planificador->quantum){
		//Checkeo si termino su quantum pero sigue necesitando asi lo encolo
		if(!puedo_ejecutar()){
			encolar(planificador->entrenadorEnExec);
		}
		//Hago el paquete Pop, entrenadorEnExec, Estado a Exec, y signal del sem
		t_entrenador* entrenador = queue_pop(planificador->cola);
		planificador->entrenadorEnExec = entrenador;
		entrenador->estado = EXEC;
		planificador->quantum_actual = 1;
		sem_post(&(entrenador->semaforo));
	    //Checkeo si es la primera iteracion
		}else if(planificador->quantum_actual == 0){
			//Hago el paquete Pop, entrenadorEnExec, Estado a Exec, y signal del sem
			t_entrenador* entrenador = queue_pop(planificador->cola);
			planificador->entrenadorEnExec = entrenador;
			entrenador->estado = EXEC;
			planificador->quantum_actual++;
			sem_post(&(entrenador->semaforo));
		}else{
			//Checkeo si termino por blockeo y no por quantum
			if(puedo_ejecutar()){
				//Hago el paquete Pop, entrenadorEnExec, Estado a Exec, y signal del sem
				planificador->quantum_actual = 0;
				t_entrenador* entrenador = queue_pop(planificador->cola);
				planificador->entrenadorEnExec = entrenador;
				entrenador->estado = EXEC;
				planificador->quantum_actual++;
				sem_post(&(entrenador->semaforo));
			//Caso normal de que le queda quantum y necesita ejecutar
			}else{
				planificador->quantum_actual++;
				sem_post(&(planificador->entrenadorEnExec->semaforo));
			}
		}
}
