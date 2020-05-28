#include "planificacion_fifo.h"

void planificar_fifo(){
	//En un planificador fifo, la re-planificacion se da solo
	//cuando el proceso actual deja de ejecutar
	//En este sistema nosotros podemos saber el proceso actual
	//a traves del planificador, y podemos acceder a su estado
	//para saber si sigue en EXEC o no.
	//(el estado cambia una vez que el entrenador envio su mensaje CATCH)

	if(hay_alguien_en_ejecucion()){
		//El entrenador sigue en ejecucion, entonces debo
		//disparar el signal de su semaforo, para evitar que este
		//se bloquee en el proximo ciclo
		sem_post(&(planificador->entrenador_en_exec->semaforo));
	}else{
		//El entrenador termino de ejecutar, por lo tanto
		//debo sacarlo de ejecucion en el planificador
		//y no hacer el signal, de esa forma al comenzar
		//el proximo ciclo quedara bloqueado
		sacar_de_ejecucion();
		//Tambien podria pasar que no habia nadie en ejecucion, entonces
		//sigo de la misma manera (solo aclaro para no olvidarnos de esto)

		//Luego debo comprobar si hay algun entrenador en cola
		//asi lo puedo poner en ejecucion (actualizando el planificador
		//y disparando su signal para que inicie el ciclo)

		if(!queue_is_empty(planificador->cola)){
			t_entrenador* entrenador = queue_pop(planificador->cola);
			entrar_a_ejecucion(entrenador);
			sem_post(&(entrenador->semaforo));
		}
	}
}
