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

void cerrarArchivos(FILE* arch[],size_t ce){
    for(short int i=0;i<ce;i++)
        fclose(arch[i]);
}

bool crearArchivos(FILE* arch[],String* path,const size_t ce,const char* apertura){
    for(short int i=0;i<ce;i++){
        arch[i] = fopen(path[i].vec,apertura);
        if(!arch[i]){
            cerrarArchivos(arch,i);
            return ERROR_AL_ABRIR_ARCHIVOS;
        }
    }
    return TODO_OK;
}

t_pixel** leerBMP(t_metadata* meta_bmp,FILE* arch[],String* funci,char* path,size_t *tam){

    FILE* BMP=fopen(path,RB);
    if(!BMP)
        return NULL;

    if(crearArchivos(arch,funci,*tam,WB)){
        fclose(BMP);
        return NULL;
    }

    EncabezadoBMP bits;
    fread(&bits,sizeof(EncabezadoBMP),1,BMP);

    for(size_t i=0;i<*tam;i++)// Copio el encabezado en todos los archivos creados
        fwrite(&bits,sizeof(EncabezadoBMP),1,arch[i]);


    meta_bmp->alto=bits.alto;
    meta_bmp->ancho=bits.ancho;
    meta_bmp->profundidad=bits.tamPuntos;
    meta_bmp->tamArchivo=bits.tamArch;
    meta_bmp->tamEncabezado=bits.tamCabecera;

    t_pixel** mat = (t_pixel**)matrizCrear(meta_bmp->alto,meta_bmp->ancho,sizeof(t_pixel));

    if(!mat){
        fclose(BMP);
        cerrarArchivos(arch,*tam);
        return NULL;
    }

    size_t padding = (4 - (meta_bmp->ancho * 3) % 4) % 4;// El padding se calcula para que cada fila sea un múltiplo de 4 bytes de longitud
    size_t i=0;
    while(!feof(BMP) && i<meta_bmp->alto){
        for(size_t j=0;j<meta_bmp->ancho;j++){
            fread(&mat[i][j],sizeof(char),3,BMP);
            mat[i][j].profundidad=meta_bmp->profundidad;
        }
        fseek(BMP,padding,SEEK_CUR);
        i++;
    }

    fclose(BMP);
    printf("%s\n",path);
    return mat;
}


void negativo(FILE* prin, FILE* modi){
    uint8_t pixel[3];
    while(!feof(prin)){
        fread(&pixel,sizeof(pixel),1,prin);
        pixel[0] = 255-pixel[0];
        pixel[1] = 255-pixel[1];
        pixel[2] = 255-pixel[2];
        fwrite(&pixel,sizeof(pixel),1,modi);
    }
    fclose(modi);
}

void escala_de_grises(FILE* prin, FILE* modi){
    uint8_t pixel[3];
    while(!feof(prin)){
        fread(&pixel,sizeof(pixel),1,prin);
        uint8_t prom =(pixel[0]+pixel[1]+pixel[2])/3;
        pixel[0] = prom;
        pixel[1] = prom;
        pixel[2] = prom;
        fwrite(&pixel,sizeof(pixel),1,modi);
    }
    fclose(modi);
}

void tonalidad(t_pixel** mat, FILE* modi,short tono){
    uint8_t pixel[3];
    while(!feof(prin)){
        fread(&pixel,sizeof(pixel),1,prin);
        pixel[tono]*=1.5;
        fwrite(&pixel,sizeof(pixel),1,modi);
    }
    fclose(modi);
}

void rotar_derecha(FILE* prin, FILE* modi){

}

void rotar_izquierda(FILE* prin, FILE* modi){

}

void solucion(int argc,char* argv[])
{
    if(argc<=1){ // Pregunto cuantos argumentos hay
        puts("No hay Argumentos suficientes");
        exit(SIN_PARAMETROS);
    }
    size_t cantFun=argc-2;
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

    t_metadata meta_bmp;

    FILE* archivos[cantFun];

    t_pixel** mat = leerBMP(&meta_bmp,archivos,funci,archBMP.vec,&cantFun);

    if(mat){
        //negativo(mat,archivos[0],&meta_bmp);
        //escala_de_grises(mat,archivos[0]);
        tonalidad(mat,archivos[0],VERDE);
    }

    //Elimino Tipos de Datos
    stringEliminar(&archBMP);
    for(j=0;j<cantFun;j++)
        stringEliminar(&(funci[j]));

}
