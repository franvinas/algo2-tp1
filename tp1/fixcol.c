#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../strutil/strutil.h"

int main(int argc,char* argv[]) {
  if(argc != 3) {
    printf("Error en la cantidad de parametros\n");
    return 0;
  }

  FILE* archivo = fopen(argv[1], "r");
  if(!archivo) {
    printf("No se pudo leer el archivo\n");
    return 0;
  }
  char* linea = NULL;
  char* linea_nueva = NULL;
  size_t tam_linea = atoi(argv[2]);
  size_t capacidad;
  size_t leidos;
  size_t escritos = 0;

  leidos = getline(&linea, &capacidad, archivo);
  while(!feof(archivo)) {
    escritos = 0;

    while(escritos < leidos) {

      linea_nueva = substr(linea + escritos,tam_linea);

      printf("%s", linea_nueva);

      escritos += tam_linea;

      if(escritos < leidos -1)
        printf("\n");

      free(linea_nueva);
    }

    leidos = getline(&linea, &capacidad, archivo);
  }

  fclose(archivo);

  return 0;
}
