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

bool __ampliarCapVector(Vector*);
bool __ampliarString(String*,size_t);
size_t largoString(void*);
void matrizElim(void**,const size_t);

bool crearString(String* vec){
    vec->vec = malloc(sizeof(char));

    if(!vec->vec)
        return SIN_MEMORIA;

    vec->ce=0;
    vec->cap=sizeof(char);
    return TODO_OK;
}

bool insertarString(String* vec,char* pal){
    size_t tam = largoString(pal);

    if(__ampliarString(vec,tam+vec->ce))
        return SIN_MEMORIA;

    char* j = vec->vec+vec->ce;

    for(size_t i=0;i<=tam;i++){
        (*j)=pal[i];
        j++;
    }
    vec->ce+=tam;
    return TODO_OK;
}

bool __ampliarString(String* vec,size_t tam){

    void* nvec = realloc(vec->vec,tam);

    if(!nvec)
        return SIN_MEMORIA;

    vec->vec=nvec;
    vec->cap=tam;
    return TODO_OK;
}

bool crearVector(Vector* vec,size_t tamElem){
    vec->vec = malloc(CAP_INI*tamElem);

    if(!vec->vec)
        return SIN_MEMORIA;

    vec->cap=CAP_INI;
    vec->ce=0;
    vec->tamElem=tamElem;

    return TODO_OK;
}

bool vectorInsertarFinal(Vector* vec,const void* elem){
    if(vec->cap==vec->ce)
        if(!__ampliarCapVector(vec))
            return SIN_MEMORIA;

    void* posIns = vec->vec+vec->ce * vec->tamElem;
    memcpy(posIns,elem,vec->tamElem);
    vec->ce++;

    return TODO_OK;
}

bool __ampliarCapVector(Vector* vec){
    size_t cap = vec->cap*FACTOR_INCR;
    void* nvec=realloc(vec->vec,cap*vec->tamElem);

    if(!nvec)
        return SIN_MEMORIA;

    vec->vec=nvec;
    vec->cap=cap;

    return TODO_OK;
}

size_t largoString(void* pal){
    size_t tam=0;
    char* j = pal;
    while(*j!='\0'){
        tam++;
        j++;
    }
    return tam;
}

void vectorElim(Vector* vec){
    free(vec->vec);
    vec->vec = NULL;
}

void stringEliminar(String* vec){
    free(vec->vec);
    vec->vec=NULL;
}

void** matrizCrear(size_t filas, size_t columnas,size_t tamElem){
    void** matriz = malloc(filas*sizeof(void*)); // Creo primer vector de filas

    if(!matriz)// Mira que no falle el malloc
        return NULL;

    void** ult=matriz+filas-1;// Ultimo elemento del vector de filas

    for(void** i=matriz;i<=ult;i++){
        *i= malloc(columnas*tamElem);// Creo los vectores de columnas

        if(!i){// Prueba que no falle el malloc de las columnas, en tal caso borra todo lo creado hasta el momento
            matrizElim(matriz,i-matriz);
            return NULL;
        }
    }

    return matriz;
}

void matrizElim(void** mat,const size_t fil){
    void** ult= mat+fil-1;

    for(void** i=mat;i<=ult;i++){
        free(*i);
    }
    free(mat);
}

void crearArchivos(FILE* arch[],String* path,const size_t ce,const char* apertura){

    for(short int i=0;i<ce;i++){
        arch[i] = fopen(path[i].vec,apertura);
        if(!arch[i]){
            for(short int j=0;j<i;j++)
                fclose(arch[j]);
        }
    }
}

void cerrarArchivos(FILE* arch[],size_t ce){
    for(short int i=0;i<ce;i++)
        fclose(arch[i]);
}

void solucion(int argc,char* argv[])
{
    if(argc<=1){ // Pregunto cuantos argumentos hay
        puts("No hay Argumentos suficientes");
        exit(SIN_PARAMETROS);
    }
    short int cantFun=argc-2;
    String funci[cantFun];//Funciones a realizar
    String archBMP;//Para el Nombre del archivo

    //Creo tipos TDA String
    crearString(&archBMP);
    for(short int i=0;i<cantFun;i++)
        crearString(&(funci[i]));

    short int j=0;
    //Saco funciones y nombre de los archivos
    for(short int i=1;i<argc;i++){
        if(argv[i][0]=='-'){
            insertarString(&(funci[j]),"Estudiante_");
            insertarString(&(funci[j]),&argv[i][2]);
            insertarString(&(funci[j]),".bmp");
            j++;
        }else
            insertarString(&archBMP,argv[i]);
    }

    //Creo archivos a utilizar
    FILE* archivos[cantFun];
    crearArchivos(archivos,funci,cantFun,WB);

    if(archivos[0]!=NULL){

        FILE* BMP=fopen(archBMP.vec,RB);

        if(!BMP)
            exit(ARCHIVO_NO_ENCONTRADO);

        char bits;

        fread(&bits,sizeof(char),1,BMP);
        while(!feof(BMP)){
            printf("%x",bits);
            fread(&bits,sizeof(char),1,BMP);
        }

        //Cierro el archivo principal
        fclose(BMP);

        cerrarArchivos(archivos,cantFun);
    }
    //Elimino Tipos de Datos
    stringEliminar(&archBMP);
    for(j=0;j<cantFun;j++)
        stringEliminar(&(funci[j]));

}
