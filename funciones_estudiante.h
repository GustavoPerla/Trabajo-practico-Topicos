#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED

//Bibliotecas
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//Headers
#include "constantes.h"
#include "estructuras.h"

//Macros utiles
#define CAP_INI 10
#define FACTOR_INCR 1.5
#define RB "rb"   // Apertura de archivo
#define WB "wb"  // Apertura de archivo
#define AZUL 0
#define VERDE 1
#define ROJO 2
#define AUMENTAR 1// Contraste
#define REDUCIR -1// Contraste

//Macros de errores
#define SIN_MEMORIA 5
#define SIN_PARAMETROS 15
#define FUNC_INEXI 25
#define ERROR_AL_ABRIR_ARCHIVOS 30
#define REPETIDO 27


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

typedef struct{
    uint16_t nom;
    uint32_t tamArch;
    uint32_t reservado;
    uint32_t offset;
    uint32_t tamCabecera;
    uint32_t ancho;
    uint32_t alto;
    uint16_t numplanos;
    uint16_t tamPuntos;
    uint32_t compresion;
    uint32_t tamImagen;
    uint32_t resolucionH;
    uint32_t resolucionV;
    uint32_t tamTablaColor;
    uint32_t contColImpo;
}EncabezadoBMP;

void solucion(int argc, char* argv[]);
size_t largoString(void*);
void matrizElim(void**,const size_t);


#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
