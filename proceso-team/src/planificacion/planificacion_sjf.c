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

}

//TODO consulta:
//Actualizo las ultimas estimaciones siempre o solo cuando lo elijo???

//Estimacion proxima: Rafaga anterior * alpha + Est anterior * (1 - alpha)
t_entrenador* shortest_job(){

	t_list* entrenadores_en_ready = planificador->cola->elements;
	t_entrenador* entrenador_mas_corto = list_get(entrenadores_en_ready, 0);
	int index_mas_corto = 0;

	for(int i = 0 ; i < list_size(entrenadores_en_ready) ; i++){
		t_entrenador* entrenador = list_get(entrenadores_en_ready, i);
		entrenador->estado_sjf->ultima_estimacion = calcular_estimacion(entrenador);

		if(entrenador->estado_sjf->ultima_estimacion < entrenador_mas_corto->estado_sjf->ultima_estimacion){
			entrenador_mas_corto = entrenador;
			index_mas_corto = i;
		}
	}

	return list_remove(planificador->cola->elements, index_mas_corto);
}

double calcular_estimacion(t_entrenador* entrenador){
	double raf = entrenador->estado_sjf->ultima_rafaga * planificador->alpha;
	double est = entrenador->estado_sjf->ultima_estimacion * (1 - planificador->alpha);

	printf("Estimando entrenador %d: ult raf = %d, ult est = %f, nueva est = %f\n",
			entrenador->identificador, entrenador->estado_sjf->ultima_rafaga,
			entrenador->estado_sjf->ultima_estimacion, raf + est);

	return raf + est;
}
