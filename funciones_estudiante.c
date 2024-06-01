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
void tonalidad(t_pixel,FILE*,short);
void negativo(t_pixel,FILE*);
void escala_de_grises(t_pixel,FILE*);
void rotar_derecha(FILE*, t_pixel**, uint32_t, uint32_t);


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

        if(!*i){// Prueba que no falle el malloc de las columnas, en tal caso borra todo lo creado hasta el momento
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

void copiarEncabezado(FILE* arch[],size_t* tam,EncabezadoBMP bits){
    for(size_t i=0;i<*tam;i++){// Copio el encabezado en todos los archivos creados
        fseek(arch[i],0,SEEK_SET);
        fwrite(&bits.nom,2,1,arch[i]);
        fwrite(&bits.tamArch,sizeof(int),1,arch[i]);
        fwrite(&bits.reservado,sizeof(int),1,arch[i]);
        fwrite(&bits.offset,sizeof(int),1,arch[i]);
        fwrite(&bits.tamCabecera,sizeof(int),1,arch[i]);
        fwrite(&bits.ancho,sizeof(int),1,arch[i]);
        fwrite(&bits.alto,sizeof(int),1,arch[i]);
        fwrite(&bits.numplanos,sizeof(short),1,arch[i]);
        fwrite(&bits.tamPuntos,sizeof(short),1,arch[i]);
        fwrite(&bits.compresion,sizeof(int),1,arch[i]);
        fwrite(&bits.tamImagen,sizeof(int),1,arch[i]);
        fwrite(&bits.resolucionH,sizeof(int),1,arch[i]);
        fwrite(&bits.resolucionV,sizeof(int),1,arch[i]);
        fwrite(&bits.tamTablaColor,sizeof(int),1,arch[i]);
        fwrite(&bits.contColImpo,sizeof(int),1,arch[i]);
    }
}

t_metadata leerEcabezado(FILE* BMP,FILE* arch[],size_t* tam){
    EncabezadoBMP bits;
    t_metadata meta;
    fseek(BMP,0,SEEK_SET);

    fread(&bits.nom,2,1,BMP);
    fread(&bits.tamArch,sizeof(int),1,BMP);
    fread(&bits.reservado,sizeof(int),1,BMP);
    fread(&bits.offset,sizeof(int),1,BMP);
    fread(&bits.tamCabecera,sizeof(int),1,BMP);
    fread(&bits.ancho,sizeof(int),1,BMP);
    fread(&bits.alto,sizeof(int),1,BMP);
    fread(&bits.numplanos,sizeof(short),1,BMP);
    fread(&bits.tamPuntos,sizeof(short),1,BMP);
    fread(&bits.compresion,sizeof(int),1,BMP);
    fread(&bits.tamImagen,sizeof(int),1,BMP);
    fread(&bits.resolucionH,sizeof(int),1,BMP);
    fread(&bits.resolucionV,sizeof(int),1,BMP);
    fread(&bits.tamTablaColor,sizeof(int),1,BMP);
    fread(&bits.contColImpo,sizeof(int),1,BMP);

    //Saco datos importantes
    meta.alto=bits.alto;
    meta.ancho=bits.ancho;
    meta.profundidad=bits.tamPuntos;
    meta.tamArchivo=bits.tamArch;
    meta.tamEncabezado=bits.tamCabecera;

    copiarEncabezado(arch,tam,bits);

    return meta;
}


t_pixel** leerBMP(t_metadata* meta_bmp,FILE* arch[],String* funci,char* path,size_t *tam){

    FILE* BMP=fopen(path,RB);
    if(!BMP)
        return NULL;

    if(crearArchivos(arch,funci,*tam,WB)){
        fclose(BMP);
        return NULL;
    }

    *meta_bmp = leerEcabezado(BMP,arch,tam);

    t_pixel** mat = (t_pixel**)matrizCrear(meta_bmp->alto,meta_bmp->ancho,sizeof(t_pixel));

    if(!mat){
        fclose(BMP);
        cerrarArchivos(arch,*tam);
        return NULL;
    }

    size_t padding = (4 - (meta_bmp->ancho * 3) % 4) % 4;// El padding se calcula para que cada fila sea un múltiplo de 4 bytes de longitud
    size_t i=0;
    fseek(BMP,54,SEEK_SET);
    while(i<meta_bmp->alto){
        for(size_t j=0;j<meta_bmp->ancho;j++){
            fread(&mat[i][j],3,1,BMP);
            mat[i][j].profundidad=meta_bmp->profundidad;
            //tonalidad(mat[i][j],arch[0],VERDE);
            //negativo(mat[i][j],arch[0]);
            escala_de_grises(mat[i][j],arch[1]);
        }
        fseek(BMP,padding,SEEK_CUR);
        i++;
    }
    rotar_derecha(arch[0],mat,meta_bmp->alto,meta_bmp->ancho);
    cerrarArchivos(arch,*tam);
    fclose(BMP);
    return mat;
}


void negativo(t_pixel pix,FILE* arch){
    pix.pixel[0] = 255-pix.pixel[0];
    pix.pixel[1] = 255-pix.pixel[1];
    pix.pixel[2] = 255-pix.pixel[2];
    fwrite(&pix,3,1,arch);
}

void escala_de_grises(t_pixel pix,FILE* arch){
    uint8_t prom =(pix.pixel[0]+pix.pixel[1]+pix.pixel[2])/3;
    pix.pixel[0] = prom;
    pix.pixel[1] = prom;
    pix.pixel[2] = prom;
    fwrite(&pix,3,1,arch);
}

void tonalidad(t_pixel pix,FILE* arch,short tono){
    if(pix.pixel[tono]*1.5>255)
        pix.pixel[tono]=255;
    else
        pix.pixel[tono]*=1.5;
    fwrite(&pix,3,1,arch);
}

void rotar_derecha(FILE* arch, t_pixel** mat,uint32_t fila,uint32_t col){
    fseek(arch,18,SEEK_SET);
    fwrite(&fila,sizeof(int),1,arch);
    fwrite(&col,sizeof(int),1,arch);
    fseek(arch,54,SEEK_SET);

    fila--;
    col--;
    for(size_t i=col;i<0;i--)
        for(size_t j=fila;j>0;j--)
            fwrite(mat[j][i].pixel,3,1,arch);
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
        matrizElim((void**)mat,meta_bmp.alto);
    }

    //Elimino Tipos de Datos
    stringEliminar(&archBMP);
    for(j=0;j<cantFun;j++)
        stringEliminar(&(funci[j]));

}
