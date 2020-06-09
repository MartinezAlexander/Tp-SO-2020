#include "planificacion_rr.h"

void planificar_rr(){
	//En un planificador ruond robin, la planificacion se da
	//en dos casos:
	//1. Cuando el proceso actual termina de ejecutar
	//2. Cuando se termina el quantum

	if(hay_alguien_en_ejecucion()){
		//Primero que nada aumento el quantum, ya que acabo de ejecutar
		planificador->quantum_actual++;

		//Ahora que se que estaba ejecutando alguien
		//debo ver si se le termino su quantum o no,
		//en ese caso, saco el actual (encolandolo de nuevo)
		//y ejecuto el que sigue
		if(planificador->quantum_actual == planificador->quantum){
			encolar(planificador->entrenador_en_exec);
			pasar_proximo_a_ejecucion();
		}else{
			//Puedo seguir ejecutando, que nosotros
			//lo permitimos dando el signal de su
			//semaforo para que el proximo ciclo no
			//lo corte
			sem_post(&(planificador->entrenador_en_exec->semaforo));
		}
	}else{
		//No hay nadie ejecutando o
		//termino de ejecutar el actual
		pasar_proximo_a_ejecucion();
	}
}

void pasar_proximo_a_ejecucion(){
	//Aca entro de dos maneras posibles
	//Si termino el que estaba ejecutando.
	//en ese caso, solo lo saco del planificador
	//(y al no hacer el signal, ya deja de ejecutar)
	sacar_de_ejecucion();
	//Luego tambien chequare si hay alguien
	//disponible para ejecutar en cola

	//O si no habia nadie ejecutando.
	//en este caso chequeo que haya alguien en cola
	//y lo ejecuto, setteando su quantum a cero.
	if(!queue_is_empty(planificador->cola)){
		t_entrenador* entrenador = queue_pop(planificador->cola);
		entrar_a_ejecucion(entrenador);
		planificador->quantum_actual = 0;
		sem_post(&(entrenador->semaforo));
	}
}
