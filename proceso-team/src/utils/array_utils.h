/*
 * array_utils.h
 *
 *  Created on: 13 may. 2020
 *      Author: utnso
 */

#ifndef ARRAY_UTILS_H_
#define ARRAY_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/collections/list.h>

t_list* array_to_list(char** array);
int array_cantidad_de_elementos(char** array);
void free_string_array(char** string_array);


#endif /* ARRAY_UTILS_H_ */
