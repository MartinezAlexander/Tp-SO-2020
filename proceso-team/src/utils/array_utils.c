#include "array_utils.h"

t_list* array_to_list(char** array){

	t_list* lista = list_create();
	for(int i = 0; array[i]!=NULL ; i++){
		list_add(lista,array[i]);
	}
	return lista;
}

int array_cantidad_de_elementos(char** array){
	int cantidad = 0;
	while(array[cantidad] != NULL) cantidad++;
	return cantidad;
}

void free_string_array(char** string_array){
	int i;
	for(i = 0 ; string_array[i] ; i++){
		free(string_array[i]);
	}

	free(string_array);
}
