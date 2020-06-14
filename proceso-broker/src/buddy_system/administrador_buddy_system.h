#ifndef BUDDY_SYSTEM_ADMINISTRADOR_BUDDY_SYSTEM_H_
#define BUDDY_SYSTEM_ADMINISTRADOR_BUDDY_SYSTEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <mensajes/mensajes.h>
#include "buddy_system.h"

t_buddy* buddy_principal;

void iniciar_buddy_system();
t_list* obtener_mensajes_cacheados_por_cola_bs(op_code cola);
void bs_cachear_mensaje(t_mensaje* mensaje);

//int es_hora_de_compactar(int tamanio_a_guardar);


#endif /* BUDDY_SYSTEM_ADMINISTRADOR_BUDDY_SYSTEM_H_ */
