    /*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el último campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido: Perla
    Nombre: Gustavo Tobias
    DNI: 43386388
    Entrega: Si
    -----------------
    Apellido:Ibarra
    Nombre: Gustavo
    DNI: 42887359
    Entrega: Si
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
*/

#include "funciones_estudiante.h"

#define EXISTENTESCOL(pal) !compararString(pal,"escala-de-grises")\
                        || !compararString(pal,"aumentar-contraste")\
                        || !compararString(pal,"reducir-contraste")\
                        || !compararString(pal,"tonalidad-roja")\
                        || !compararString(pal,"tonalidad-azul")\
                        || !compararString(pal,"tonalidad-verde")\
                        || !compararString(pal,"negativo")

#define EXISTENTESMAT(pal) !compararString(pal,"rotar-total")\
                        || !compararString(pal,"rotar-derecha")\
                        || !compararString(pal,"rotar-izquierda")\
                        || !compararString(pal,"comodin")\
                        || !compararString(pal,"recortar")

bool __ampliarString(String*,size_t);
size_t largoString(void*);
void matrizElim(void**,const size_t);
void tonalidad(const unsigned char[],FILE*,short);
void negativo(const unsigned char [],FILE*);
void escala_de_grises(const unsigned char[],FILE*);
void contraste(const unsigned char pixel[],FILE*,short);
void rotar(FILE*, t_pixel**, int32_t, int32_t,size_t,short);
void recortar(FILE*, t_pixel** ,int32_t ,int32_t,size_t);
void comodin(FILE*,t_pixel**,int32_t,int32_t,size_t);
int compararString(const char*,const char*);
void rotar_total(FILE*,t_pixel**,int32_t,int32_t,size_t);

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

    if(__ampliarString(vec,tam+vec->cap))
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

size_t largoString(void* pal){
    size_t tam=0;
    char* j = pal;
    while(*j!='\0'){
        tam++;
        j++;
    }
    return tam;
}

void stringEliminar(String* vec){
    free(vec->vec);
    vec->vec=NULL;
}

bool stringVaciar(String* vec){
    if(__ampliarString(vec,1))
        return SIN_MEMORIA;
    vec->ce=0;
    vec->vec='\0';
    return TODO_OK;
}

short insertarStringOrd(String* fun,size_t cantFun,char* pal){
    short i=0;
    while(i<cantFun && compararString(fun[i].vec+11,pal)<0){
        i++;
    }

    if((i<cantFun && compararString(fun[i].vec+11,pal)) || i==cantFun){
        if(crearString(&fun[cantFun]))
            return SIN_MEMORIA;
        for(size_t j=cantFun;j>i;j--){
            insertarString(&fun[j],fun[j-1].vec);
            stringVaciar(&fun[j-1]);
        }
        insertarString(&fun[i],"estudiante_");
        insertarString(&fun[i],pal);
        insertarString(&fun[i],".bmp");
        return TODO_OK;
    }else
        return REPETIDO;
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

void copiarEncabezado(FILE* arch[],size_t* tam,t_metadata bits){
    for(size_t i=0;i<*tam;i++){// Copio el encabezado en todos los archivos creados
        fwrite("BM",2,1,arch[i]);
        fwrite(&bits.tamArchivo,sizeof(unsigned int),1,arch[i]);
        fseek(arch[i],10,SEEK_SET);
        uint32_t ini = bits.tamEncabezado+14;
        fwrite(&ini,sizeof(unsigned int),1,arch[i]);
        fwrite(&bits.tamEncabezado,sizeof(unsigned int),1,arch[i]);
        fseek(arch[i],18,SEEK_SET);
        fwrite(&bits.ancho,sizeof(unsigned int),1,arch[i]);
        fwrite(&bits.alto,sizeof(unsigned int),1,arch[i]);
        fseek(arch[i],28,SEEK_SET);
        fwrite(&bits.profundidad,sizeof(unsigned short),1,arch[i]);
        fseek(arch[i],34,SEEK_SET);
        size_t tam = 3*bits.alto*bits.ancho;
        fwrite(&tam,sizeof(unsigned int),1,arch[i]);
    }
}

t_metadata leerEcabezado(FILE* BMP){
    t_metadata meta;

    fseek(BMP,2,SEEK_SET);
    fread(&meta.tamArchivo,sizeof(int),1,BMP);
    fseek(BMP,14,SEEK_SET);
    fread(&meta.tamEncabezado,sizeof(int),1,BMP);
    fread(&meta.ancho,sizeof(int),1,BMP);
    fread(&meta.alto,sizeof(int),1,BMP);
    fseek(BMP,28,SEEK_SET);
    fread(&meta.profundidad,sizeof(short),1,BMP);
    return meta;
}

int compararString(const char* vec,const char* pal){//Devuelve 0 si son iguales, negativo si vec es men, positivo si es mayor
    while(*vec && (*vec==*pal) && *vec!='.'){
        vec++;
        pal++;
    }
    if(*vec=='.'){
        vec--;
        pal--;
    }
    return (*vec-*pal);
}

void funciones(const unsigned char* rgb,FILE* arch[],String* col,size_t tam){//Funciones de cambio de colores
    size_t i=0;
    if(i<tam && !compararString(col[i].vec+11,"aumentar-contraste")){
        contraste(rgb,arch[i],AUMENTAR);
        i++;
    }
    if(i<tam && !compararString(col[i].vec+11,"escala-de-grises")){
        escala_de_grises(rgb,arch[i]);
        i++;
    }
    if(i<tam && !compararString(col[i].vec+11,"negativo")){
        negativo(rgb,arch[i]);
        i++;
    }
    if(i<tam && !compararString(col[i].vec+11,"reducir-contraste")){
        contraste(rgb,arch[i],REDUCIR);
        i++;
    }
    if(i<tam && !compararString(col[i].vec+11,"tonalidad-azul")){
        tonalidad(rgb,arch[i],AZUL);
        i++;
    }
    if(i<tam && !compararString(col[i].vec+11,"tonalidad-roja")){
        tonalidad(rgb,arch[i],ROJO);
        i++;
    }
    if(i<tam && !compararString(col[i].vec+11,"tonalidad-verde")){
        tonalidad(rgb,arch[i],VERDE);
    }
}

void funcionesMat(FILE* archMat[],t_pixel** mat,unsigned int* alto,unsigned int* ancho,size_t* ini,String* funMat,size_t tam){//Funciones que requieren matriz
    size_t i=0;

    if(i<tam && !compararString(funMat[i].vec+11,"comodin")){
        comodin(archMat[i],mat,*alto,*ancho,*ini);
        i++;
    }
    if(i<tam && !compararString(funMat[i].vec+11,"recortar")){
        recortar(archMat[i],mat,*alto,*ancho,*ini);
        i++;
    }
    if(i<tam && !compararString(funMat[i].vec+11,"rotar-derecha")){
        rotar(archMat[i],mat,*alto,*ancho,*ini,DERECHA);
       i++;
    }
    if(i<tam && !compararString(funMat[i].vec+11,"rotar-izquierda")){
        rotar(archMat[i],mat,*alto,*ancho,*ini,IZQUIERDA);
        i++;
    }
    if(i<tam && !compararString(funMat[i].vec+11,"rotar-total")){
        rotar_total(archMat[i],mat,*alto,*ancho,*ini);
        i++;
    }
}

void pad(size_t tam,FILE* arch){//Padding
    size_t padding = (4 - (tam * 3) % 4) % 4;
    uint8_t z=0;

    for(int k=0;k<padding;k++)
        fwrite(&z,sizeof(uint8_t),1,arch);
}

short leerBMP(String* funMat,String* col,char* path,size_t* cantMat,size_t* cantCol){

    FILE* BMP=fopen(path,RB);
    if(!BMP)
        return ARCHIVO_NO_ENCONTRADO;

    FILE* archCol[*cantCol];
    FILE* archMat[*cantMat];

    if(crearArchivos(archMat,funMat,*cantMat,WB)){
        fclose(BMP);
        return ERROR_AL_ABRIR_ARCHIVOS;
    }

     if(crearArchivos(archCol,col,*cantCol,WB)){
        fclose(BMP);
        cerrarArchivos(archMat,*cantMat);
        return ERROR_AL_ABRIR_ARCHIVOS;
    }

    t_metadata meta_bmp = leerEcabezado(BMP);

    copiarEncabezado(archCol,cantCol,meta_bmp);
    copiarEncabezado(archMat,cantMat,meta_bmp);

    size_t ini=meta_bmp.tamEncabezado+14;

    for(size_t i=0;i<*cantCol;i++)
        fseek(archCol[i],ini,SEEK_SET);

    t_pixel** mat=NULL;
    if(*cantMat>0){
        mat = (t_pixel**)matrizCrear(meta_bmp.alto,meta_bmp.ancho,sizeof(t_pixel));
        if(!mat){
            fclose(BMP);
            cerrarArchivos(archMat,*cantMat);
            cerrarArchivos(archCol,*cantCol);
            return SIN_MEMORIA;
        }
    }

    size_t padding = (4 - (meta_bmp.ancho * 3) % 4) % 4;// El padding se calcula para que cada fila sea un múltiplo de 4 bytes de longitud

    int i=0;
    t_pixel RGB;
    RGB.profundidad=meta_bmp.profundidad;
    fseek(BMP,ini,SEEK_SET);
    while(i<meta_bmp.alto){
        for(int j=0;j<meta_bmp.ancho;j++){
            fread(&RGB.pixel,3,1,BMP);
            if(*cantCol>0)
                funciones(RGB.pixel,archCol,col,*cantCol);
            if(*mat)
                mat[i][j]=RGB;
        }
        if(padding)
            for(int j=0;j<*cantCol;j++)
                pad(meta_bmp.ancho,archCol[j]);
        fseek(BMP,padding,SEEK_CUR);
        i++;
    }
    fclose(BMP);
    if(*mat){
        funcionesMat(archMat,mat,&meta_bmp.alto,&meta_bmp.ancho,&ini,funMat,*cantMat);
        matrizElim((void**)mat,meta_bmp.alto);
    }
    cerrarArchivos(archMat,*cantMat);
    cerrarArchivos(archCol,*cantCol);
    return TODO_OK;
}

void negativo(const unsigned char pixel[],FILE* arch){
    unsigned char pix[3];
    pix[0] = 255-pixel[0];
    pix[1] = 255-pixel[1];
    pix[2] = 255-pixel[2];
    fwrite(&pix,3,1,arch);
}

void escala_de_grises(const unsigned char pixel[],FILE* arch){
    unsigned char pix[3];

    uint8_t prom =(pixel[0]+pixel[1]+pixel[2])/3;
    pix[0] = prom;
    pix[1] = prom;
    pix[2] = prom;
    fwrite(&pix,3,1,arch);
}

void tonalidad(const unsigned char pixel[],FILE* arch,short tono){
    unsigned char pix[3];
    pix[0]=pixel[0];
    pix[1]=pixel[1];
    pix[2]=pixel[2];

    if(pixel[tono]*1.5>255)
        pix[tono]=255;
    else
        pix[tono]=pixel[tono]*1.5;
    fwrite(&pix,3,1,arch);
}

void rotar(FILE* arch, t_pixel** mat,int32_t fila,int32_t col,size_t ini,short opc){
    fseek(arch,18,SEEK_SET);
    fwrite(&fila,sizeof(int),1,arch);
    fwrite(&col,sizeof(int),1,arch);
    fseek(arch,ini,SEEK_SET);

    size_t senCol, senFil;
    if(!opc){
        senCol=col;
        senFil=1;
    }else
        if(opc==1){
            senCol=1;
            senFil=fila;
        }

    for(int i=0;i<col;i++){
        for(int j=0;j<fila;j++)
            fwrite(&mat[abs(senFil-1-j)][abs(senCol-1-i)].pixel,3,1,arch);
        pad(fila,arch);
    }
}

void rotar_total(FILE* arch,t_pixel** mat,int32_t fila,int32_t col,size_t ini){
    fseek(arch,ini,SEEK_SET);

    for(int i=fila-1;i>=0;i--){
        for(int j=col-1;j>=0;j--)
            fwrite(&mat[i][j].pixel,3,1,arch);
        pad(fila,arch);
    }
}

void recortar(FILE* arch, t_pixel** mat,int32_t fila,int32_t col,size_t ini){
    fseek(arch,18,SEEK_SET);
    int f=fila/2, c=col/2;
    fwrite(&c,sizeof(fila),1,arch);
    fwrite(&f,sizeof(col),1,arch);
    fseek(arch,ini,SEEK_SET);

    for(int i=fila/2;i<fila;i++){
        for(int j=0;j<col/2;j++)
            fwrite(&mat[i][j].pixel,3,1,arch);
        pad(col/2,arch);
    }
}

void contraste(const unsigned char pixel[],FILE* arch,short opcion){
    unsigned char pix[3];
    int16_t byte;

    byte = pixel[0] + ((pixel[0]-128)*.25)*opcion;
    pix[0] = (byte > 255) ? 255 : (byte < 0) ? 0 : byte;

    byte = pixel[1] + ((pixel[1]-128)*.25)*opcion;
    pix[1] = (byte > 255) ? 255 : (byte < 0) ? 0 : byte;

    byte = pixel[2] + ((pixel[2]-128)*.25)*opcion;
    pix[2] = (byte > 255) ? 255 : (byte < 0) ? 0 : byte;

    fwrite(&pix,3,1,arch);
}

void comodin(FILE* arch, t_pixel** mat,int32_t fila,int32_t col,size_t ini){
    fseek(arch,ini,SEEK_SET);

    for(int i = 0 ; i < fila ; i++){
        for(int j = col-1 ; j>=0 ; j--)
            fwrite(&mat[i][j].pixel,3,1,arch);
        pad(col,arch);
    }
}

bool funciExistente(const char *pal){
    return EXISTENTESCOL(pal) || EXISTENTESMAT(pal);
}

void solucion(int argc,char* argv[]){

    if(argc<=2){ // Pregunto cuantos argumentos hay
        return;
    }

    String col[7],mat[5];//Funciones a realizar
    String archBMP;//Para el Nombre del archivo

    if(!crearString(&archBMP)){
        bool x;
        size_t k=0,t=0,p=0,i=1;
        //Saco funciones y nombre de los archivos
        while(i<argc && p!=SIN_MEMORIA){
            if(argv[i][0]=='-'){
                if(funciExistente(&argv[i][2])){
                    if(EXISTENTESMAT(&argv[i][2])){
                        p=insertarStringOrd(mat,k,&argv[i][2]);
                        if(!p)
                            k++;
                    }else{
                        p=insertarStringOrd(col,t,&argv[i][2]);
                        if(!p)
                            t++;
                    }
                }
            }else{
                p=insertarString(&archBMP,argv[i]);
                if(!p)
                    x=true;
            }
            i++;
        }
        if(p!=SIN_MEMORIA && (k>0 || t>0)){
            leerBMP(mat,col,archBMP.vec,&k,&t);
        }
    //Elimino Tipos de Datos
        if(x)
            stringEliminar(&archBMP);
        for(size_t j=0;j<t;j++)
            stringEliminar(&col[j]);
        for(size_t j=0;j<k;j++)
            stringEliminar(&mat[j]);
    }
}
