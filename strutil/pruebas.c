#include "strutil.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimir_strv(char** strv) {
  size_t i = 0;

  while(strv[i]) {
    printf("%zu) %s\n", i, strv[i]);
    i++;
  }
}

void prueba_substr() {

  // PRUEBA 1
  char* cadena = "hola mundo";
  char* cadena2 = substr(cadena, 6);
  print_test("Hola mundo → Hola m", !strcmp("hola m", cadena2));



  free(cadena2);


  // PRUEBA 2
  cadena = "ejemplo";

  printf("Resultado esperado: substr(ejemplo + 2, 2) → em\n");

  cadena2 = substr(cadena + 2, 2);

  printf("Resultado: %s\n", cadena2);

  free(cadena2);

  // PRUEBA 3
  printf("Resultado esperado: substr( , 2) →  \n");

  cadena = substr("", 2);

  printf("Resultado: %s\n", cadena);

  free(cadena);

  // PRUEBA 4
  printf("Resultado esperado: substr(Algoritmos, 30) → Algoritmos \n");

  cadena = substr("Algoritmos", 30);

  printf("Resultado: %s\n", cadena);

  free(cadena);
}

void prueba_split() {

  char* cadena = "hola como va";

  printf("Cadena: %s\n", cadena);
  printf("Largo: %li\n", strlen(cadena));

  char** strv = split(cadena, ' ');
  imprimir_strv(strv);

  free_strv(strv);

}

void prueba_join() {

  char** strv_vacio = NULL;
  char* cadena_vacia = join(strv_vacio, '_');

  printf("Tam en bytes: %li\n", sizeof(cadena_vacia));

  print_test("Prueba con un strv vacio", !strlen(cadena_vacia));
  free(cadena_vacia);

  char** strv = malloc(sizeof(char*) * 6);
  strv[0] = substr("hola", 0);
  strv[1] = substr("chau", 4);
  strv[2] = substr("hawaii", 6);
  strv[3] = substr("sisi",4);
  strv[4] = substr("as",2);
  strv[5] = NULL;

  printf("Se quiere unir el siguiente arreglo de string con el separador _\n");
  imprimir_strv(strv);

  char* cadena = join(strv, '_');

  printf("Resultado: %s\n", cadena);
  printf("Largo de la cadena: %li\n", strlen(cadena));

  free_strv(strv);
  free(cadena);

  // JOIN CON STRV DE 1 ELEMENTO
  char** strv_un_elemento = malloc(sizeof(char*) * 2);
  strv_un_elemento[0] = substr("abc", 3);
  strv_un_elemento[1] = NULL;

  char* cadena_un_elemento = join(strv_un_elemento, '_');

  print_test("Prueba de join de una sola palabra", !strcmp(cadena_un_elemento, strv_un_elemento[0]));

  free_strv(strv_un_elemento);
  free(cadena_un_elemento);

  // ELEMENTO NULL
  char** strv_un_elemento_null = malloc(sizeof(char*));
  strv_un_elemento_null[0] = NULL;

  char* cadena_un_elemento_null = join(strv_un_elemento_null, '_');

  print_test("Prueba de join de una sola palabra", !strcmp(cadena_un_elemento_null, ""));

  free_strv(strv_un_elemento_null);
  free(cadena_un_elemento_null);

}

int pruebas_alumno() {

  // printf("\nPRUEBA SUBSTR\n\n");
  // prueba_substr();
  //
  // printf("\nPRUEBA SPLIT\n\n");
  // prueba_split();

  printf("\nPRUEBA JOIN\n\n");
  prueba_join();


  return 0;
}
