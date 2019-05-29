#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "pila.h"

#define TAM_INICIAL 11
#define CANTIDAD_INSTRUCCIONES 8
#define FACTOR_REDIMENSION 2

#define AVANZAR '>'
#define RETROCEDER '<'
#define AUMENTAR '+'
#define DECRECER '-'
#define ESCRIBIR '.'
#define LEER ','
#define LOOP_INICIO '['
#define LOOP_FIN ']'

/************************************************************************
                          FUNCIONES AUXILIARES
*************************************************************************/
int* inicializar_vector_instrucciones() {
  int* instrucciones = malloc(CANTIDAD_INSTRUCCIONES * sizeof(int));
  if(!instrucciones) return NULL;

  instrucciones[0] = AVANZAR;
  instrucciones[1] = RETROCEDER;
  instrucciones[2] = AUMENTAR;
  instrucciones[3] = DECRECER;
  instrucciones[4] = ESCRIBIR;
  instrucciones[5] = LEER;
  instrucciones[6] = LOOP_INICIO;
  instrucciones[7] = LOOP_FIN;

  return instrucciones;
}

bool es_instruccion_valida(int* instrucciones, int caracter) {
  size_t largo = CANTIDAD_INSTRUCCIONES;
  int valor;
  for(size_t i = 0; i < largo; i++) {
    valor = instrucciones[i];
    if(valor == caracter) return true;
  }
  return false;
}

int* almacenar_script_en_vector(FILE* script, size_t* n_script) {

  int* v_script = malloc(TAM_INICIAL * sizeof(int));
  if(!v_script) return NULL;
  *n_script = TAM_INICIAL;

  int* instrucciones = inicializar_vector_instrucciones();
  if(!instrucciones) {
    free(v_script);
    return NULL;
  }
  int actual;
  size_t insertados = 0;
  while(!feof(script)) {
    actual = fgetc(script);
    if(es_instruccion_valida(instrucciones, actual)) {
      v_script[insertados] = actual;
      insertados++;
      if(insertados == *n_script) {
        *n_script *= FACTOR_REDIMENSION;
        v_script = realloc(v_script, *n_script * sizeof(int));
      }
    }
  }
  *n_script = insertados;
  free(instrucciones);
  return v_script;
}

// Pone ceros en todas las posiciones del vector entre inicio y fin
void inicializar_vector_ceros(unsigned char* vector, size_t inicio, size_t fin) {
  for(size_t i = inicio; i < fin; i++) {
    vector[i] = 0;
  }
}

size_t pos_final_loop(int* vector, size_t pos) {
  int valor_actual;
  bool se_encontro = false;
  while(!se_encontro) {
    pos++;
    valor_actual = vector[pos];
    if(valor_actual == LOOP_INICIO)
      pos = pos_final_loop(vector, pos);

    if(valor_actual == LOOP_FIN)
      se_encontro = true;
  }
  return pos;
}
/************************************************************************
                          PROGRAMA PRINCIPAL
*************************************************************************/

int main(int argc, char* argv[]) {

  if(argc > 3) {
    printf("Cantidad erronea de parametros\n");
    return 0;
  }

  FILE* script;
  if(argc > 1) {
    script = fopen(argv[1], "r");
    if(!script) {
      printf("No se pudo leer el archivo\n");
      return 0;
    }
  }
  else {
    script = fopen("script.txt", "w");
    char* linea = NULL;
    size_t capacidad = 0;
    size_t leidos = getline(&linea, &capacidad, stdin);
    while(leidos > 1) {
      fprintf(script, "%s", linea);
      leidos = getline(&linea, &capacidad, stdin);
    }

    fclose(script);
    script = fopen("script.txt", "r");
  }

  FILE* entrada;

  if(argc <= 2) {
    entrada = stdin;
  } else {

    entrada = fopen(argv[1], "r");
    if(!entrada) {
      printf("No se pudo abrir el archivo de entrada\n");
      fclose(script);
      return 0;
    }
  }

  FILE* salida = stdout;

  pila_t* pila_loop = pila_crear();
  if(!pila_loop) {
    fclose(script);
    fclose(entrada);
    return 0;
  }

  unsigned char* vector = malloc(TAM_INICIAL * sizeof(unsigned char));
  size_t tam_vector = TAM_INICIAL;
  size_t posicion = 0;
  if(!vector) {
    pila_destruir(pila_loop);
    fclose(script);
    fclose(entrada);
    return 0;
  }
  inicializar_vector_ceros(vector, 0, tam_vector);

  size_t n_script;
  int* v_script = almacenar_script_en_vector(script, &n_script);
  if(!v_script) {
    free(vector);
    fclose(script);
    fclose(entrada);
    return 0;
  }
  fclose(script); // ya tengo todo almacenado en un vector asi que no necesito el archivo

  size_t* pos_loop_inicio;
  int comando;
  unsigned char valor_actual;
  for(size_t i = 0; i < n_script; i++) {
    comando = v_script[i];
    switch (comando) {
      case AVANZAR:
        posicion++;
        if(posicion == tam_vector) {
          tam_vector *= FACTOR_REDIMENSION;
          vector = realloc(vector, tam_vector * sizeof(unsigned char));
          inicializar_vector_ceros(vector, posicion, tam_vector);
        }
        break;

      case RETROCEDER:
        posicion--;
        break;

      case AUMENTAR:
        vector[posicion]++;
        break;

      case DECRECER:
        vector[posicion]--;
        break;

      case ESCRIBIR:
        valor_actual = vector[posicion];
        fputc(valor_actual, salida);
        break;

      case LEER:
        valor_actual = (unsigned char) fgetc(entrada);
        vector[posicion] = valor_actual;
        break;

      case LOOP_INICIO:
        valor_actual = vector[posicion];
        if(valor_actual == 0) { // termina el loop
          i = pos_final_loop(v_script, i);
          break;
        }
        pos_loop_inicio = pila_ver_tope(pila_loop);
        if(!pos_loop_inicio || i != *pos_loop_inicio) {
          pos_loop_inicio = malloc(sizeof(size_t));
          *pos_loop_inicio = i;
          pila_apilar(pila_loop, pos_loop_inicio);
        }
        break;
      case LOOP_FIN:
        valor_actual = vector[posicion];
        if(valor_actual == 0) { // termina el loop
          pos_loop_inicio = pila_desapilar(pila_loop);
          free(pos_loop_inicio);
        } else {
          pos_loop_inicio = pila_ver_tope(pila_loop);
          i = *pos_loop_inicio;
        }
        break;

    }

  }

  pila_destruir(pila_loop);
  free(vector);
  free(v_script);
  fclose(entrada);
  fclose(salida);
  return 0;
}
