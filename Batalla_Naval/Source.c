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
    char tableroPrincipal[FILAS][COLUMNAS];     //VISIBLE, con posiciones descubiertas
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
void GODMODE(Partida* partida); //testing y cheats

//Funciones de posicions
int zonaLibre(int tablero[FILAS][COLUMNAS], int fila, int columna); //punto aleatoreo libre alrededor
void colocarFragatas(Partida* partida); //genero barco? lo guardo:falla inento en todas las posiciones(sino vuelvo atras
void Puntoaleatorio(Barco* barco);
void Direccionesaleatorias(int direcciones[4]);
int barcoLibre(Partida* partida, Barco* barco);
void colocarBarco(Partida* nueva, int indice, int tamanio);


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
        system("mode con: cols=120 lines=45"); //Terminal de tamano fijo
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
    /* Variables Locales*/
    int caracter;
    int lectura;


    int godmode = 1; //1 cheats o 0 normal TESTING
    int continuar = 1;
    int comandoInvalido = 0;
    char comando[30];

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
        colocarFragatas(&nueva); 

        do
        {
            system("cls");
            printf("Jugador: %s\n", nueva.nombre);
            printf("Disparos restantes: %d\n", nueva.disparosRestantes);
            
            
            if (godmode == 1) {
                GODMODE(&nueva);
            }
            else {
                mostrarTablero(nueva.tableroPrincipal);
            }

            if (comandoInvalido == 1) {
                printf("\nComando incorrecto.\n");
            }
            comandoInvalido = 0;

            printf("\ngodmode_1 / godmode_0 / volver\n");   //TESTING
            printf("Ingrese un comando: ");

            if (fgets(comando, sizeof(comando), stdin) == NULL) {
                continuar = 0;
            }
            else {
                comando[strcspn(comando, "\n")] = '\0';

                if (strcmp(comando, "godmode_1") == 0) {
                    godmode = 1;
                }
                else if (strcmp(comando, "godmode_0") == 0) {
                    godmode = 0;
                }
                else if (strcmp(comando, "volver") == 0) {
                    continuar = 0;
                }
                else {
                    comandoInvalido = 1;
                }
            }
        } while (continuar == 1);
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
            //Declaro para evitar UB
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
int zonaLibre(int tablero[FILAS][COLUMNAS], int fila, int columna) {
    /* Variables Locales*/
    int filaVecina;
    int columnaVecina;
    int libre = 0;

    if (fila >= 0 && fila < FILAS && columna >= 0 && columna < COLUMNAS) {
        libre = 1;
    }
    for (filaVecina = fila - 1; filaVecina <= fila + 1; filaVecina++)
    {
        for (columnaVecina = columna - 1;
            columnaVecina <= columna + 1;
            columnaVecina++)
        {
            if (filaVecina >= 0 && filaVecina < FILAS &&
                columnaVecina >= 0 && columnaVecina < COLUMNAS)
            {
                if (tablero[filaVecina][columnaVecina] != 0)
                {
                    libre = 0;
                }
            }
        }
    }
    return libre;
}

void GODMODE(Partida* partida)
{
    char vista[FILAS][COLUMNAS];
    int fila;
    int columna;

    for (fila = 0; fila < FILAS; fila++)
    {
        for (columna = 0; columna < COLUMNAS; columna++)
        {
            vista[fila][columna] = partida->tableroPrincipal[fila][columna];

            if (partida->tableroPosicion[fila][columna] != 0 &&
                vista[fila][columna] == 'O')
            {
                vista[fila][columna] = 'B';
            }
        }
    }

    printf("\nGODMODE - B indica un barco oculto\n");
    mostrarTablero(vista);
}


void colocarFragatas(Partida* nueva) {
    /*Variables locales*/
    int i;
    int fila;
    int columna;
    int indice;
    int intento;
    int casilla;
    int libre;
    int direcciones[4];
    int cambioFila[4] = { -1, 1, 0, 0 };
    int cambioColumna[4] = { 0, 0, -1, 1 };
    Barco* barco;

    //FRAGATAS 1 casilla
    for (i = 0; i < Cant_Fragatas; i++)
    {
        do
        {
            Puntoaleatorio(&nueva->barcos[i]);
            fila = nueva->barcos[i].fila;
            columna = nueva->barcos[i].columna;
        } while (zonaLibre(nueva->tableroPosicion, fila, columna) == 0);

        nueva->tableroPosicion[fila][columna] = i + 1;
        nueva->barcos[i].tamanio = 1;
        nueva->barcos[i].impactos = 0;
        nueva->barcos[i].direccionFila = 0;
        nueva->barcos[i].direccionColumna = 0;
    }
    // Destructores 2 casillas
    for (i = 0; i < Cant_Destructores; i++){
        indice = Cant_Fragatas + i; //barcos con index 4,5,6
        colocarBarco(nueva, indice, 2);
    }

    // Submarinos 3 casillas
    for (i = 0; i < Cant_Submarinos; i++){
        indice = Cant_Fragatas + Cant_Destructores + i; //7,8
        colocarBarco(nueva, indice, 3);
    }

    // Portaaviones 4 casillas
    for (i = 0; i < Cant_Portaaviones; i++){
        indice = Cant_Fragatas + Cant_Destructores + Cant_Submarinos + i; 
        colocarBarco(nueva, indice, 4);
    }

}
void colocarBarco(Partida* nueva, int indice, int tamanio)
{
    int fila;
    int columna;
    int casilla;
    int intento;
    int libre = 0;
    int direcciones[4];
    int cambioFila[4] = { -1, 1, 0, 0 };
    int cambioColumna[4] = { 0, 0, -1, 1 };
    Barco* barco = &nueva->barcos[indice];

    barco->tamanio = tamanio;
    barco->impactos = 0;

    do
    {
        Puntoaleatorio(barco);
        Direccionesaleatorias(direcciones);
        intento = 0;

        while (intento < 4 && libre == 0)
        {
            barco->direccionFila = cambioFila[direcciones[intento]];
            barco->direccionColumna = cambioColumna[direcciones[intento]];
            libre = barcoLibre(nueva, barco);
            intento++;
        }
    } while (libre == 0);

    for (casilla = 0; casilla < tamanio; casilla++)
    {
        fila = barco->fila + casilla * barco->direccionFila;
        columna = barco->columna + casilla * barco->direccionColumna;
        nueva->tableroPosicion[fila][columna] = indice + 1;
    }
}

int barcoLibre(Partida* partida, Barco* barco)
{
    int casilla = 0;
    int fila;
    int columna;
    int libre = 1;

    while (casilla < barco->tamanio && libre == 1)
    {
        fila = barco->fila + casilla * barco->direccionFila;
        columna = barco->columna + casilla * barco->direccionColumna;

        if (zonaLibre(partida->tableroPosicion, fila, columna) == 0) {
            libre = 0;
        }

        casilla++;
    }

    return libre;
}

void Puntoaleatorio(Barco* barco) {
        barco->fila = rand() % FILAS;
        barco->columna = rand() % COLUMNAS;
    
}

void Direccionesaleatorias(int direcciones[4]) {
    int i;
    int posicion;
    int auxiliar;
    //[0,1,2,3] 
    for (i = 0; i < 4; i++) {
        direcciones[i] = i;
    }
    //mezclo el array
    for (i = 3; i > 0; i--) {
        posicion = rand() % (i + 1);
        auxiliar = direcciones[i];
        direcciones[i] = direcciones[posicion];
        direcciones[posicion] = auxiliar;
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

void Quinta(void){
    FILE* arch;
    int caracter;

    arch = fopen("../Saludo.txt", "r");

    if (arch == NULL) {
        printf("No se pudo abrir Saludo.txt.\n");
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

    printf("\n\033[1;33m");
    printf("  +=============================================================================+\n");
    printf("  |                                                                             |\n");
    printf("  |                         HASTA LA PROXIMA, CAPITAN!                          |\n");
    printf("  |                                                                             |\n");
    printf("  +=============================================================================+\n");
    printf("\n\033[0m");
}

