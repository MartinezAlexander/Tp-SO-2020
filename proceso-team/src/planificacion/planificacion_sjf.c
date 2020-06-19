#include "planificacion_sjf.h"


void planificar_sjf_sd(){
	//Un planificador SJF (sin desalojo) replanifica cuando:
	//Termina un proceso

	if(hay_alguien_en_ejecucion()){
		//Debe seguir ejecutando el actual, entonces disparo su
		//semaforo para que no se bloquee, ademas actualizo su ultima rafaga
		planificador->entrenador_en_exec->estado_sjf->ultima_rafaga++;
		sem_post(&(planificador->entrenador_en_exec->semaforo));
	}else{
		sacar_de_ejecucion();

		if(!queue_is_empty(planificador->cola)){
			//Calculo la estimacion mas corta y
			//reseteo la rafaga del entrenador elegido
			t_entrenador* entrenador = shortest_job();
			entrar_a_ejecucion(entrenador);
			entrenador->estado_sjf->ultima_rafaga = 1;
			sem_post(&(entrenador->semaforo));
		}
	}
}



//TODO planificacion sjf con desalojo
void planificar_sjf_cd(){
	//Replanifica cuando:
	//Termina proceso
	//Llega uno NUEVO a la cola => en entrar a planificacion, me fijo si uso cd => planifico (esta comentado)


}
//Solucion para SJF CD
//El algoritmo va a ser casi igual salvo el cambio de la variable esta

//Entonces, cada objeto sjf va a tener una variable
//que represente si empece a ejecutar y no termine
//Entonces, si es 0 tengo que calcular como siempre
//Si es 1 significa que para mi ultima estimacion no termine de ejecutar lo que estime
//entonces calculo haciendo la resta ultima_est - ultima_raf

//La variable esta la pongo en 1 cuando lo pongo en EXEC
//Y la paso a 0 cuando termino de ejecutar





//Estimacion proxima: Rafaga anterior * alpha + Est anterior * (1 - alpha)
t_entrenador* shortest_job(){

	t_list* entrenadores_en_ready = planificador->cola->elements;

	int index_mas_corto = 0;
	double estimacion_mas_corta = 100;

	for(int i = 0 ; i < list_size(entrenadores_en_ready) ; i++){
		t_entrenador* entrenador = list_get(entrenadores_en_ready, i);
		double estimacion = calcular_estimacion(entrenador);

		if(estimacion < estimacion_mas_corta){
			index_mas_corto = i;
			estimacion_mas_corta = estimacion;
		}
	}

	t_entrenador* entrenador_mas_corto = list_remove(planificador->cola->elements, index_mas_corto);
	//Solo actualizamos la estimacion al que sabemos que va a ejecutar,
	//los demas quedan con la estimacion anterior
	entrenador_mas_corto->estado_sjf->ultima_estimacion = estimacion_mas_corta;
	return entrenador_mas_corto;
}

double calcular_estimacion(t_entrenador* entrenador){
	double raf = entrenador->estado_sjf->ultima_rafaga * planificador->alpha;
	double est = entrenador->estado_sjf->ultima_estimacion * (1 - planificador->alpha);
/*
	printf("Estimando entrenador %d: ult raf = %d, ult est = %f, nueva est = %f\n",
			entrenador->identificador, entrenador->estado_sjf->ultima_rafaga,
			entrenador->estado_sjf->ultima_estimacion, raf + est);
*/
	return raf + est;
}
