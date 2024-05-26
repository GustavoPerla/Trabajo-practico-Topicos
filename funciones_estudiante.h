#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED

//Bibliotecas
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


//Headers
#include "constantes.h"
#include "estructuras.h"

//Macros utiles
#define CAP_INI 10
#define FACTOR_INCR 1.5
#define RB "rb"   // Apertura de archivo
#define WB "wb"  // Apertura de archivo

//Macros de errores
#define SIN_MEMORIA 5
#define SIN_PARAMETROS 15
#define FUNC_INEXI 25


typedef struct{
    void* vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}Vector;

typedef struct{
    void* vec;
    size_t ce;
    size_t cap;
}String;



void solucion(int argc, char* argv[]);
size_t largoString(void*);
void matrizElim(void**,const size_t);


#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
