#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"

int main(int argc,char* argv[]) {
  if(argc != 3) {
    fprintf(stderr, "%s","Error: Cantidad erronea de parametros\n");
    return 0;
  }

  FILE* archivo = fopen(argv[1], "r");
  if(!archivo) {
    fprintf(stderr, "%s","Error: archivo fuente inaccesible\n");
    return 0;
  }
  char* linea = NULL;
  char* linea_nueva = NULL;
  size_t tam_linea = atoi(argv[2]);
  size_t capacidad;
  size_t leidos;
  size_t escritos = 0;

  leidos = getline(&linea, &capacidad, archivo);
  while(leidos != -1) {
    linea[leidos-1] = linea[leidos-1] == '\n' ? '\0' : linea[leidos-1];
    size_t largo = strlen(linea);
    escritos = 0;

    while(escritos < largo) {
      linea_nueva = substr(linea + escritos,tam_linea);
      printf("%s\n", linea_nueva);
      escritos += tam_linea;
      free(linea_nueva);
    }

    free(linea);
    linea = NULL;
    capacidad = 0;
    leidos = getline(&linea, &capacidad, archivo);
  }

  free(linea);
  fclose(archivo);

  return 0;
}
