#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED

#include "constantes.h"
#include "estructuras.h"

#define CAP_INI 10
#define FACTOR_INCR 1.5
#define SIN_PARAMETROS 15

typedef struct{
    void* vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}Vector;

void solucion(int* argc, char* argv[]);



#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
