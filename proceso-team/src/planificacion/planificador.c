#include "planificador.h"

tipo_planificacion obtener_algoritmo(char* algoritmo){
	if(string_equals_ignore_case(algoritmo,"FIFO")) return FIFO;
	else if(string_equals_ignore_case(algoritmo,"RR")) return RR;
	else if(string_equals_ignore_case(algoritmo,"SJF-CD")) return SJF_CD;
	else if(string_equals_ignore_case(algoritmo,"SJF-SD")) return SJF_SD;
	else{
		printf("\n[Planificacion] --Algoritmo de planificacion invalido--\n\n");
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
	planificador->debo_desalojar_al_fin_de_ciclo = 0;
	sem_init(&(planificador->semaforo_desalojo), 0, 0);
	sem_init(&(planificador->semaforo_post_desalojo), 0, 0);
	pthread_mutex_init(&(planificador->mutex_desalojo), NULL);
	sem_init(&(semaforo_planificacion), 0, 0);
	pthread_mutex_init(&(planificador->mutex_planificacion), NULL);
	pthread_mutex_init(&(planificador->mutex_entrenador), NULL);
	planificador->quantum_actual = 0;
	pthread_mutex_init(&mutex_pokemones_ready, NULL);
    return planificador;
}

int hay_alguien_en_ejecucion(){
	pthread_mutex_lock(&planificador->mutex_entrenador);
	if(planificador->entrenador_en_exec == NULL){
		pthread_mutex_unlock(&planificador->mutex_entrenador);
		return 0;
	}
	else {
		int resultado = entrenador_en_ejecucion(planificador->entrenador_en_exec);
		pthread_mutex_unlock(&planificador->mutex_entrenador);
		return resultado;
	}
}

void entrar_a_ejecucion(t_entrenador* entrenador){
	pthread_mutex_lock(&planificador->mutex_entrenador);
	planificador->entrenador_en_exec = entrenador;
	pthread_mutex_unlock(&planificador->mutex_entrenador);

	pthread_mutex_lock(&entrenador->mutex_lectura_entrenador);
	entrenador->estado = EXEC;
	pthread_mutex_unlock(&entrenador->mutex_lectura_entrenador);
	//Estadisticas
	cambios_de_contexto++;

	loggear_planificacion(entrenador->identificador);
}

void sacar_de_ejecucion(){
	//Estadisticas
	pthread_mutex_lock(&planificador->mutex_entrenador);
	if(planificador->entrenador_en_exec != NULL){
		cambios_de_contexto++;
	}
	planificador->entrenador_en_exec = NULL;
	pthread_mutex_unlock(&planificador->mutex_entrenador);
}

void encolar(t_entrenador* entrenador)
{
	printf("[Planificacion] Encolado entrenador %d en [%d , %d]\n", entrenador->identificador, entrenador->posicion.posicionX,  entrenador->posicion.posicionY);

	pthread_mutex_lock(&entrenador->mutex_lectura_entrenador);
	entrenador->estado = READY;
	pthread_mutex_unlock(&entrenador->mutex_lectura_entrenador);

	pthread_mutex_lock(&mutex_pokemones_ready);
	queue_push(planificador->cola, entrenador);
	pthread_mutex_unlock(&mutex_pokemones_ready);
}

void planificador_destroy(t_planificador* p){
	queue_destroy(p->cola);

	if(p->entrenador_en_exec != NULL){
		printf("[ERROR] Error al destruir planificador, todavia guarda una referencia a entrenador\n");
	}

	pthread_mutex_destroy(&p->mutex_desalojo);
	pthread_mutex_destroy(&p->mutex_planificacion);

	sem_destroy(&p->semaforo_desalojo);
	sem_destroy(&p->semaforo_post_desalojo);

	free(p);
}
