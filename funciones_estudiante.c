    /*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el último campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido: Perla
    Nombre: Gustavo Tobias
    DNI: 43386388
    Entrega: Si
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
*/

#include "funciones_estudiante.h"

bool __ampliarCapVector(Vector* vec);

bool crearVector(Vector* vec,size_t tamElem){
    vec->vec = malloc(CAP_INI*tamElem);

    if(!vec->vec)
        return false;
    vec->cap=CAP_INI;
    vec->ce=0;
    vec->tamElem=tamElem;

    return true;
}

bool vectorInsertarFinal(Vector* vec){

}

bool __ampliarCapVector(Vector* vec){
    size_t cap = vec->cap*vec->tamElem
    void* nvec=realloc(vec->vec,cap*vec->tamElem);

    if(!nvec)
        return false;

    vec->vec=nvec;
    vec->cap=cap;

    return true;
}

void solucion(int argc, char* argv[])
{
    if(argc<=1){
        puts("No hay Argumentos suficientes");
        return SIN_PARAMETROS;
    }


}
