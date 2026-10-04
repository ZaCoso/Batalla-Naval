#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
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

int mostrarMenu(void);                          //0- origen
void nuevaPartida(void);                        //1- menu

void mostrarInstrucciones(void);                //3- menu


  
//Nueva partida
void inicializarTableros(Partida *partida);
void mostrarTablero(char tablero[FILAS][COLUMNAS]);



int zonaLibre(int tablero[FILAS][COLUMNAS], int fila, int columna);
void colocarFragatas(Partida* partida);

int main(void)
{
    int opcion;

    //semilla random
    srand((unsigned int)time(NULL));

    printf("BATALLA NAVAL\n");
    printf("Bienvenido, capitan!\n");

    do
    {
        opcion = mostrarMenu();

        switch (opcion)
        {
        case 1:
            nuevaPartida();
            break;
        case 2:
            printf("La recuperacion de partidas esta pendiente.\n");
            break;
        case 3:
            mostrarInstrucciones();
            break;
        case 4:
            printf("El ranking esta pendiente.\n");
            break;
        case 5:
            printf("Hasta la proxima!\n");
            break;
        default:
            printf("Ingrese una opcion del 1 al 5.\n");
        }
    } while (opcion != 5);

    return 0;
}

/* Funciones */
int leerLinea(char cadena[], int capacidad)
{
    int caracter;
    char *salto;

    if (fgets(cadena, capacidad, stdin) == NULL)
    {
        return 0;
    }

    salto = strchr(cadena, '\n');
    if (salto != NULL)
    {
        *salto = '\0';
    }
    else
    {
        /* Descarta lo que no entro para no afectar la siguiente lectura. */
        while ((caracter = getchar()) != '\n' && caracter != EOF)
        {
        }
    }

    return 1;
}

int mostrarMenu(void)
{
    char entrada[20];

    printf("\n1. Nueva partida\n");
    printf("2. Recuperar partida guardada\n");
    printf("3. Ver instrucciones\n");
    printf("4. Ver ranking\n");
    printf("5. Salir\n");
    printf("Opcion: ");

    if (!leerLinea(entrada, sizeof(entrada)))
    {
        return 5;
    }

    if (strlen(entrada) == 1 && entrada[0] >= '1' && entrada[0] <= '5')
    {
        return entrada[0] - '0';
    }

    return 0;
}

void nuevaPartida(void)
{
    Partida partida = {0};

    printf("\nNombre del jugador: ");
    if (!leerLinea(partida.nombre, sizeof(partida.nombre)))
    {
        return;
    }

    if (partida.nombre[0] == '\0')
    {
        printf("El nombre no puede estar vacio.\n");
        return;
    }

    partida.disparosRestantes = MAX_DISPAROS;
    inicializarTableros(&partida);

    printf("\nJugador: %s\n", partida.nombre);
    mostrarTablero(partida.tableroPrincipal);
    printf("La colocacion de barcos y los disparos estan pendientes.\n");
}

void inicializarTableros(Partida *partida)
{
    int fila;
    int columna;

    for (fila = 0; fila < FILAS; fila++)
    {
        for (columna = 0; columna < COLUMNAS; columna++)
        {
            /* El tablero oculto guardara el numero de cada barco; 0 es agua. */
            partida->tableroPosicion[fila][columna] = 0;
            partida->tableroPrincipal[fila][columna] = 'O';
        }
    }
}

void mostrarTablero(char tablero[FILAS][COLUMNAS])
{
    int fila;
    int columna;

    printf("   ");
    for (columna = 1; columna <= COLUMNAS; columna++)
    {
        printf("%3d", columna);
    }
    printf("\n");

    for (fila = 0; fila < FILAS; fila++)
    {
        printf("%c  ", 'A' + fila);
        for (columna = 0; columna < COLUMNAS; columna++)
        {
            printf("%3c", tablero[fila][columna]);
        }
        printf("\n");
    }
}

void mostrarInstrucciones(void)
{
    FILE *archivo;
    char linea[200];

    archivo = fopen("instrucciones.txt", "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir instrucciones.txt.\n");
        return;
    }

    printf("\n");
    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        printf("%s", linea);
    }

    fclose(archivo);
}
