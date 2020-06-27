#include "planificacion_sjf.h"

//La planificacion SJF sin desalojo funciona exactamente igual que FIFO
//la unica diferencia es al momento de poner en ejecucion al proximo
//entrenador, ya que aca tenemos que elegir el que tiene menor estimacion
void planificar_sjf_sd(){

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


//Para la implementacion de SJF con desalojo, elegimos seguir el mismo modelo que
//para la sin desalojo, pero con dos diferencias.

//La primera es que aca voy a replanificar tambien cuando llega alguien nuevo a la cola
//en ese sentido esta parte del codigo no va a cambiar, sino que cuando le asigno un pokemon
//a un entrenador disponible y lo agrego a la cola del planificador, tendre una llamada
//mas a planificar(), ademas cuando hago esta llamada, debo desalojar el actual y ponerlo en cola

//La otra diferencia es en el calculo de estimaciones, ya que ahora puede darse que un
//entrenador no termino de ejecutar porque fue desalojado, entonces no debo calcular la
//estimacion de nuevo, sino que debo ver cuanto le resta ejecutar de lo que habia estimado
//cuando lo elegi.
//Para lograr esto, la idea es marcar a cada entrenador que empezo a ejecutar y desmarcarlo
//cuando termina. De esta manera puedo identificar a los que tienen que hacer su calculo
//en base a cuanto le falta ejecutar. Si no esta marcado, calculo normalmente.
void planificar_sjf_cd(){
	//Replanifica cuando:
	//Termina proceso
	//Llega uno NUEVO a la cola => en entrar a planificacion, me fijo si uso cd => planifico (esta comentado)

	if(hay_alguien_en_ejecucion()){
		//Debe seguir ejecutando el actual, entonces disparo su
		//semaforo para que no se bloquee, ademas actualizo su ultima rafaga
		planificador->entrenador_en_exec->estado_sjf->ultima_rafaga++;
		sem_post(&(planificador->entrenador_en_exec->semaforo));
	}else{
		//Cuando saco a alguien de ejecucion, debo desmarcarlo asi le informo que
		//el calculo de estimacion va a ser normal
		if(planificador->entrenador_en_exec != NULL){
			planificador->entrenador_en_exec->estado_sjf = 0;
		}
		//Una vez que lo desmarque, ya lo puedo sacar del planificador
		sacar_de_ejecucion();

		if(!queue_is_empty(planificador->cola)){
			//Calculo la estimacion mas corta y
			//reseteo la rafaga del entrenador elegido
			t_entrenador* entrenador = shortest_job_con_desalojo();
			entrar_a_ejecucion(entrenador);
			entrenador->estado_sjf->ultima_rafaga = 1;
			//Marco al entrenador a ejecutar
			entrenador->estado_sjf->empezo_a_ejecutar = 1;
			sem_post(&(entrenador->semaforo));
		}
	}
}




//Para encontrar el entrenador con menos estimacion recorremos todos los
//entrenadores de la cola, por cada uno calculamos su estimacion y encontramos
//al que tenga menor estimacion, luego actualizamos el valor de ultima estimacion
//para el entrenador elegido.

//Los otros entrenadores no deben actualizar su calculo, ya que como no van a
//ejecutar, su ultima rafaga no va a cambiar entonces si actualizara su estimacion,
//la proxima vez que calcule voy a estar calculando con valores incorrectos
t_entrenador* shortest_job(){
	printf("[Replanificacion] Empieza el calculo de estimaciones\n");
	t_list* entrenadores_en_ready = planificador->cola->elements;

	int index_mas_corto = 0;
	double estimacion_mas_corta = 100;

	for(int i = 0 ; i < list_size(entrenadores_en_ready) ; i++){
		t_entrenador* entrenador = list_get(entrenadores_en_ready, i);
		double estimacion = calcular_estimacion(entrenador);
		printf("[Calculo Estimacion] Entrenador %d => Estimacion: %f\n", entrenador->identificador, estimacion);
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



//TODO: ver forma de no repetir codigo
t_entrenador* shortest_job_con_desalojo(){
	printf("[Replanificacion] Empieza el calculo de estimaciones\n");

	t_list* entrenadores_en_ready = planificador->cola->elements;

	int index_mas_corto = 0;
	double estimacion_mas_corta = 100;

	for(int i = 0 ; i < list_size(entrenadores_en_ready) ; i++){
		t_entrenador* entrenador = list_get(entrenadores_en_ready, i);
		double estimacion = calcular_estimacion_con_desalojo(entrenador);
		printf("[Calculo Estimacion] Entrenador %d => Estimacion: %f\n", entrenador->identificador, estimacion);
		if(estimacion < estimacion_mas_corta){
			index_mas_corto = i;
			estimacion_mas_corta = estimacion;
		}
	}

	t_entrenador* entrenador_mas_corto = list_remove(planificador->cola->elements, index_mas_corto);

	//Solo actualizamos la estimacion al que sabemos que va a ejecutar,
	//los demas quedan con la estimacion anterior,
	//Siempre y cuando elegi uno que no habia comenzado a ejecutar
	if(entrenador_mas_corto->estado_sjf->empezo_a_ejecutar == 0){
		entrenador_mas_corto->estado_sjf->ultima_estimacion = estimacion_mas_corta;
	}

	return entrenador_mas_corto;
}



//Para el calculo con desalojo chequeo si el entrenador esta marcado como ejecutando
//en ese caso el calculo simplemente va a ser lo que le queda por ejecutar de lo que
//habia estimado
//En caso contrario, calculo normalmente
double calcular_estimacion_con_desalojo(t_entrenador* entrenador){
	if(entrenador->estado_sjf->empezo_a_ejecutar == 1){
		printf("[Calculo Estimacion] Proximo entrenador a calcular: calcula por resto\n");
		return entrenador->estado_sjf->ultima_estimacion - (double)entrenador->estado_sjf->ultima_rafaga;
	}else{
		return calcular_estimacion(entrenador);
	}
}

//Estimacion proxima: Rafaga anterior * alpha + Est anterior * (1 - alpha)
double calcular_estimacion(t_entrenador* entrenador){
	double raf = entrenador->estado_sjf->ultima_rafaga * planificador->alpha;
	double est = entrenador->estado_sjf->ultima_estimacion * (1 - planificador->alpha);

	return raf + est;
}
