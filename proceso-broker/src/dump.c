#include "dump.h"

char* fecha_actual(){
	time_t tiempo = time(NULL);
	struct tm* time = localtime(&tiempo);
	return string_from_format("Dump: %d/%d/%d %d:%d:%d\n",time->tm_mday, time->tm_mon + 1, time->tm_year, time->tm_hour, time->tm_min, time->tm_sec);
}

void dump(){

	FILE* archivo_dump;
	archivo_dump = fopen("../src/dump.txt", "a");
	if(archivo_dump != NULL){
		char* fecha_y_hora_dump = fecha_actual();
		//TODO free de la char* de la fecha

		fprintf(archivo_dump,"------------------------------------------------------------------------------------------\n");
		fprintf(archivo_dump,"%s",fecha_y_hora_dump);

		if(string_equals_ignore_case(algoritmo_memoria,"BS")){
			t_list* buddies = list_create();
			recolectar_hojas(buddy_principal,buddies);

			for(int i = 0; i < list_size(buddies); i++){
				t_buddy* buddy = list_get(buddies,i);
				char* string_buddy = buddy_to_string(buddy);
				fprintf(archivo_dump,"Particion %d: %s\n", i, string_buddy);
			}
			list_destroy(buddies);
		}else{

			for(int i = 0; i < list_size(particiones); i++){
				t_particion* particion = list_get(particiones, i);
				char* string_part = particion_to_string(particion);
				fprintf(archivo_dump,"Particion %d: %s\n", i, string_part);
			}
		}

	}

	fclose(archivo_dump);
}

void controlador_de_seniales(int num_senial){
	if(num_senial == SIGUSR1){
		dump();
	}
}