#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED

//Bibliotecas
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

//Headers
#include "constantes.h"
#include "estructuras.h"

//Macros utiles
#define CAP_INI 10
#define FACTOR_INCR 1.5

//Macros de errores
#define SIN_MEMORIA 5
#define SIN_PARAMETROS 15
#define FUNC_INEXI


typedef struct{
    void* vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}Vector;

typedef struct{
    void* vec;
    size_t ce;
}String;

void solucion(int* argc, char* argv[]);



#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
