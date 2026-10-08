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

typedef struct nodo
{
    char nombre[MAX_NOMBRE];
    int disparos;
    struct nodo *siguiente;
} Nodo;

/* funciones */
int leerLinea(char cadena[], int capacidad);

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
    char pausa[8];

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
            printf("\nPresione ENTER para volver al menu...");
            if (leerLinea(pausa, sizeof(pausa)) == 0)
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
    char entrada[32];
    char *fin;
    long opcion;
    int lectura;
    int entradaInvalida = 0;

    while (1)
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
        fflush(stdout);

        lectura = leerLinea(entrada, sizeof(entrada));
        if (lectura == 0)
        {
            return 5;
        }
        if (lectura == -1)
        {
            entradaInvalida = 1;
            continue;
        }

        opcion = strtol(entrada, &fin, 10);
        if (fin == entrada)
        {
            entradaInvalida = 1;
            continue;
        }

        while (*fin == ' ' || *fin == '\t' || *fin == '\r')
        {
            fin++;
        }

        if (*fin == '\0' && opcion >= 1 && opcion <= 5)
        {
            return (int)opcion;
        }

        entradaInvalida = 1;
    }
}

/* Devuelve 1 si leyo la linea, 0 al terminar la entrada y -1 si era larga. */
int leerLinea(char cadena[], int capacidad)
{
    char *salto;
    int caracter;

    if (fgets(cadena, capacidad, stdin) == NULL)
    {
        return 0;
    }

    salto = strchr(cadena, '\n');
    if (salto != NULL)
    {
        *salto = '\0';
        return 1;
    }

    caracter = getchar();
    if (caracter == '\n' || caracter == EOF)
    {
        return 1;
    }

    while ((caracter = getchar()) != '\n' && caracter != EOF)
    {
    }
    return -1;
}

void Primera(void)
{
    printf("NUEVA PARTIDA\n");
    printf("La creacion de la partida esta pendiente.\n");
}

void Segunda(void)
{
    printf("RECUPERAR PARTIDA\n");
    printf("La recuperacion de la partida esta pendiente.\n");
}

void Tercera(void)
{
    printf("INSTRUCCIONES\n");
    printf("La lectura de instrucciones.txt esta pendiente.\n");
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
