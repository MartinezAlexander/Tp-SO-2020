#include "planificacion.h"


void planificador_iniciar_hilo_entrenador(t_entrenador* entrenador){
	sem_init(&(entrenador->semaforo), 0, 0);
	pthread_create(&(entrenador->hilo), NULL, (void*)ejecutar_hilo_planificado, entrenador);
}

t_planificador* planificador_create(char* algoritmo, uint32_t quantum, uint32_t estimacion_inicial)
{
	t_planificador* planificador = malloc(sizeof(t_planificador));
	planificador->algoritmo_planificacion = algoritmo;
	planificador->quantum = quantum;
	planificador->estimacion_inicial = estimacion_inicial;
	planificador->cola = queue_create();
    return planificador;
}

//TODO prototipos de todas las funciones en .h
//TODO t_pokemon en bibliotec + funciones necesarias para pasarlo de un mensaje

void entrenador_entrar_a_planificacion(t_pokemon* pokemon){

	//Filtro entrenadores disponibles
	t_list* entrenadores_disponibles = list_filter(entrenadores, (void*)entrenador_disponible);

	//Obtengo entrenador mas cercano al pokemon a atrapar
	t_entrenador *entrenador_mas_cercano = obtener_entrenador_mas_cercano(entrenadores_disponibles, pokemon);

	//Le doy el objetivo actual al entrenador
	entrenador_mas_cercano->objetivo_actual = pokemon;

	//Agrego entrenador a la cola del planificador
	encolar(entrenador_mas_cercano);

	//Planificador comprueba si hay alguien ejecutando
		//En caso contrario pone a ejecutar al primero de la cola
	if(puedo_ejecutar()){
		ejecutar_proximo();
	}
}

//Funcion en ejecucion generica para cualquier planificador
void ejecutar_hilo_planificado(t_entrenador* entrenador){

	while(1){
		comenzar_ciclo_planificador(entrenador);

		ejecutar_entrenador(entrenador);

		finalizar_ciclo_planificador(entrenador);
	}
}

//A definir para cada planificador
void comenzar_ciclo_planificador(t_entrenador* entrenador){
	//Para FIFO
	//wait del samaforo del entrenador
	sem_wait(&(entrenador->semaforo));
}

//A definir para cada planificador
void finalizar_ciclo_planificador(t_entrenador* entrenador){

	//Para FIFO
	//Termino de ejecutar?
	if(entrenador->estado == BLOCKED){
		//Si: Pasar a ejecucion el primero en la cola
		ejecutar_proximo();
	}else{
		//No: signal del semaforo del entrenador
		sem_post(&(entrenador->semaforo));
	}
}

//A definir para cada planificador
void ejecutar_proximo(){
	//Para FIFO

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


// TODO: Hay distintos tipos de bloqueado ??
int entrenador_disponible(t_entrenador *entrenador)
{
	return entrenador->estado == NEW || entrenador->estado == BLOCKED;
}

void encolar(t_entrenador* entrenador)
{
	printf("Encolado entrenador en [%d , %d]\n", entrenador->posicion.posicionX,  entrenador->posicion.posicionY);
    entrenador->estado = READY;
	queue_push(planificador->cola, entrenador);
}

int puedo_ejecutar()
{
    return !list_any_satisfy(entrenadores, (void*)checkear_exec_entrenador);
}



t_entrenador* obtener_entrenador_mas_cercano(t_list* entrenadores, t_pokemon* pokemon)
{
	int index_mas_cercano;
	t_posicion posicion_pokemon = pokemon->posicion;
	//Ya empieza como la maxima distancia posible
	int distancia_mas_corta = posicion_pokemon.posicionX + posicion_pokemon.posicionY;

	//Comparo la cantidad de movimientos que necesito para llegar a pos.
	for(int i = 0 ; i < list_size(entrenadores) ; i++){
		t_posicion posicion = ((t_entrenador*)list_get(entrenadores, i))->posicion;

		int distancia = movimientos_entre_posiciones(posicion_pokemon, posicion);

		//En caso de empate se queda el que ya estaba (el primero)
		if(distancia < distancia_mas_corta){
			distancia_mas_corta = distancia;
			index_mas_cercano = i;
		}
	}

	return list_get(entrenadores, index_mas_cercano);
}

int movimientos_entre_posiciones(t_posicion a, t_posicion b){
	int distX = abs(a.posicionX - b.posicionX);
	int distY = abs(a.posicionY - b.posicionY);


	return distX + distY;
}
