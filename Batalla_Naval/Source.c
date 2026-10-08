#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Mis Declaraciones */
#define FILAS 10
#define COLUMNAS 10 // tablero de 10x10
#define MAX_NOMBRE 50
#define MAX_DISPAROS 30

#define Cant_Barcos 10
#define Cant_Portaaviones 1     // 4 casillas
#define Cant_Submarinos 2       // 3 casillas
#define Cant_Destructores 3     // 2 casillas
#define Cant_Fragatas 4         // 1 casillas


/* Estructuras */
typedef struct
{
    int fila;
    int columna; 
    int direccionFila;
    int direccionColumna;
    int tamanio;
    int impactos;
} Barco;
//DATOS DEL BARCO

typedef struct
{
    char nombre[MAX_NOMBRE];
    int tableroPosicion[FILAS][COLUMNAS];       //OCULTO, con todas las pos
    char tableroPrincipal[FILAS][COLUMNAS];     //VISIBLE, con pos descubiertas
    Barco barcos[Cant_Barcos];
    int disparosRealizados;
    int disparosRestantes;
    int barcosHundidos;
} Partida;
// DATOS DE LA PARTIDA

typedef struct nodo
{
    char nombre[MAX_NOMBRE];
    int disparos;
    struct nodo *siguiente;
} Nodo;
//RANKING

/* funciones */
int  mostrarMenu(void);     //0- origen
void Primera(void);         //1- Nueva partida
void Segunda(void);         //2- Cargar partida
void Tercera(void);         //3- Instrucciones
void Cuarta(void);          //4- Ranking;
void Quinta(void);          //5- Salir;


//Nueva partida
void inicializarTableros(Partida* partida);
void mostrarTablero(char tablero[FILAS][COLUMNAS]);
int zonaLibre(int tablero[FILAS][COLUMNAS], int fila, int columna);
void colocarFragatas(Partida* partida);

int main(void)
{
    int entry;
    int caracter;

    srand((unsigned int)time(NULL)); //semilla random

    do
    {
        entry = mostrarMenu();
        system("cls");

        switch (entry)
        {
        case 1:
            Primera();
            break;
        case 2:
            Segunda();
            break;
        case 3:
            Tercera();
            break;
        case 4:
            Cuarta();
            break;
        case 5:
            Quinta();
            break;
        }

        if (entry != 5)
        {
            printf("\nPresione ENTER para volver al menu...\n");
            while ((caracter = getchar()) != '\n' && caracter != EOF)
            {
            }
            if (caracter == EOF)
            {
                Quinta();
                entry = 5;
            }
        }
    } while (entry != 5);

    return 0;
}

/* Funciones */
int mostrarMenu(void)
{
    int opcion = 0;
    int lectura;
    int caracter;
    int entradaInvalida = 0;

    do
    {
        system("cls");
        printf("------------- Bienvenido a la Batalla Naval -------------\n\n");

        if (entradaInvalida)
        {
            printf("Opcion incorrecta. Ingrese un numero entre 1 y 5.\n\n");
        }

        printf("1. Nueva partida\n");
        printf("2. Recuperar partida\n");
        printf("3. Ver instrucciones\n");
        printf("4. Ver ranking\n");
        printf("5. Salir\n\n");
        printf("Ingrese una opcion: ");
        /* Lee una cifra, suficiente para las opciones del 1 al 5. */
        lectura = scanf("%1d", &opcion);
        if (lectura == EOF)
        {
            return 5;
        }

        entradaInvalida = lectura != 1 || opcion < 1 || opcion > 5;

        /* Consume el resto de la linea y rechaza texto como "1abc" o "12". */
        while ((caracter = getchar()) != '\n' && caracter != EOF)
        {
            if (caracter != ' ' && caracter != '\t' && caracter != '\r')
            {
                entradaInvalida = 1;
            }
        }
    } while (entradaInvalida != 0);

    return opcion;
}

void Primera(void)
{
    /* Variables*/
    int caracter;
    int lectura;
    /* Partida*/
    Partida nueva = {0} ; //Empiezo con todos en cero
    nueva.disparosRestantes = MAX_DISPAROS;
    inicializarTableros(&nueva);

    printf("NUEVA PARTIDA\n");
    printf("Ingrese el nombre del jugador: \n");
    lectura = scanf("%49s", nueva.nombre);
    while (lectura != 1 && lectura != EOF)
    {
        printf("No se pudo leer el nombre. Intente nuevamente:\n");
        lectura = scanf("%49s", nueva.nombre);
    }

    while ((caracter = getchar()) != '\n' && caracter != EOF)
    {
    }
    switch (lectura)
    {
    case 1:
        system("cls");
        printf("NUEVA PARTIDA\n\n");
        printf("Jugador: %s.\n", nueva.nombre);
        printf("Cantidad de disparos restantes: %d.\n", nueva.disparosRestantes);
        mostrarTablero(nueva.tableroPrincipal);
        break;
    case EOF:
        printf("Se termino la entrada.\n");
        break;
    }
}

void Segunda(void)
{
    printf("RECUPERAR PARTIDA\n");
    printf("La recuperacion de la partida esta pendiente.\n");
}


void inicializarTableros(Partida* partida)
{
    int fila;
    int columna;

    for (fila = 0; fila < FILAS; fila++)
    {
        for (columna = 0; columna < COLUMNAS; columna++)
        {
            partida->tableroPosicion[fila][columna] = 0;
            partida->tableroPrincipal[fila][columna] = 'O';
        }
    }
}

void mostrarTablero(char tablero[FILAS][COLUMNAS])
{
    int fila;
    int columna;
    /*
    -fila con numeros (1,al 10)  [5 espacios, numero, 3 espacios, REPETIR hasta 10] 
    -Separador [3 espacios, +, 3 -, +, REPETIR 10] 
    -Fila LETRAS (A numero entre | desde ( 1 al 10 columnas) [3 espacios, +, 3 -, +, REPETIR 10] 
    -Repetir Separador, Fila LETRAS hasta terminar en J con un separador

    */

    // Fila de lista de nuemros:
    printf("\n    ");
    for (columna = 0; columna < COLUMNAS; columna++)
    {
        printf("%2d  ", columna + 1);
    }
    
    //Separador:
    printf("\n   +");
    for (columna = 0; columna < COLUMNAS; columna++)
    {
        printf("---+");
    }
    printf("\n");

    for (fila = 0; fila < FILAS; fila++)
    {
        //fila LETRAS
        printf(" %c |", 'A' + fila); //Sumo a "A" para pasar de letras A-J
        for (columna = 0; columna < COLUMNAS; columna++)
        {
            printf(" %c |", tablero[fila][columna]);
        }

        //separador
        printf("\n   +");
        for (columna = 0; columna < COLUMNAS; columna++)
        {
            printf("---+");
        }
        printf("\n");
    }
}

void Tercera(void)
{
    FILE* arch;
    int caracter;

    printf("INSTRUCCIONES\n");
    arch = fopen("instrucciones.txt","r");
    if (arch == NULL) {
        printf("No se a podido abrir el archivo. \n");
    }
    else {
        caracter = fgetc(arch);
        while (feof(arch) == 0 && ferror(arch) == 0) {
            printf("%c", caracter);
            caracter = fgetc(arch);
        }

        fclose(arch);
        printf("\n");
    }
}

void Cuarta(void)
{
    printf("RANKING\n");
    printf("El ranking esta pendiente.\n");
}

void Quinta(void)
{
    printf("Hasta la proxima, capitan!\n");
}
