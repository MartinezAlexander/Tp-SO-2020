#ifndef DEADLOCK_H_
#define DEADLOCK_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include<commons/collections/queue.h>

#include "entrenador.h"

typedef struct{
	t_entrenador* entrenador;
	t_entrenador* entrenadorObjetivo;
	char* pokemonADar;
	char* pokemonARecibir;
}t_intercambio;


#endif /* DEADLOCK_H_ */
