#ifndef DEADLOCK_H_
#define DEADLOCK_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include<commons/collections/queue.h>


#include "utils/posicion_utils.h"

#include "entrenador.h"
#include "planificacion/planificacion.h"

struct t_entrenador;
typedef struct unEntrenador t_entrenador;

/*
 * Con esta estructura manejamos los intercambios.
 * La idea es ir armando items de este tipo y guardarlos en una cola
 * para poder ir ejecutandolos cuando corresponda.
 *
 * Aca esta toda la info necesaria para poder llevar a cabo un
 * intercambio por deadlock
 */
typedef struct unIntercambio{
	t_entrenador* entrenador;
	t_entrenador* entrenadorObjetivo;
	char* pokemonADar;
	char* pokemonARecibir;
}t_intercambio;



/*
 * Esta estructura surge para facilitar la deteccion de intercambios.
 * Es basicamente una copia de los entrenadores pero con los datos que son
 * necesarios.
 *
 * Nos es de utilidad ya que vamos a tener que modificar algunos datos de
 * los entrenadores a medida que buscamos intercambios, asi que hacemos una
 * copia de cada entrenador pero en esta estructura que tiene solo los datos
 * relevantes
 */
typedef struct{
	/*
	 * Al principio pensabamos guardar el id, y cuando lo necesitemos buscabamos
	 * al entrenador con tal id, pero no es de mucha ayuda, ya que nos sirve mas
	 * tener la referencia al entrenador directamente para cuando hay que armar el intercambio
	 */
	t_entrenador* entrenador;
	t_list* objetivos;
	t_list* adquiridos;
	t_posicion posicion;

}t_entrenador_copia;


/*
 * Con esta estructura nos vamos a menjar en el proceso de deteccion del mejor intercambio
 * luego al finalizar, vamos a convertir al intercambio real, que es el que nos sirve para
 * llevar a cabo la resolucion
 */
typedef struct{
	t_entrenador_copia* entrenador;
	t_entrenador_copia* entrenadorObjetivo;
	char* pokemonADar;
	char* pokemonARecibir;
}t_intercambio_copia;


t_queue* cola_intercambios_deadlock;
sem_t semaforo_resolucion_deadlock;

t_intercambio* intercambio_create(t_entrenador* entrenador, t_entrenador* entrenadorObjetivo, char* pokemonADar, char* pokemonARecibir);
t_intercambio_copia* intercambio_copia_create(t_entrenador_copia* entrenador, t_entrenador_copia* entrenadorObjetivo, char* pokemonADar, char* pokemonARecibir);
t_entrenador_copia* entrenador_copia_create(t_entrenador* entrenador);


void resolver_deadlock();
t_intercambio* primer_intercambio(t_list* entrenadores_deadlock);
t_intercambio_copia* obtener_intercambio_mas_cercano(t_list* intercambios);
t_list* filtrar_dobles_intercambios(t_list* intercambios);
int hay_doble_intercambio(t_intercambio_copia* intercambio);
t_list* obtener_posibles_intercambios(t_list* candidatos, t_entrenador_copia* entrenador_intercambio, char* especieARecibir);
t_list* filtrar_entrenadores_con_pokemon(int desde_index, t_list* entrenadores_deadlock, char* especie);
void preparar_entrenadores(t_list* entrenadoresDL);
int preparar_entrenador(t_entrenador_copia* entrenador);
int entrenador_tiene_adquirido(t_entrenador_copia* entrenador, char* especie);
int entrenador_tiene_en_objetivo(t_entrenador_copia* entrenador, char* especie);
t_intercambio* convertir_a_intercambio_real(t_intercambio_copia* intercambio_copia);
void realizar_intercambio_simbolico(t_intercambio_copia* intercambio);
void cambiar_pokemon(t_list* listado_pokemon, char* especieASacar, char* especieAMeter);

void encolar_proximo_intercambio(int primer_intercambio);









#endif /* DEADLOCK_H_ */
