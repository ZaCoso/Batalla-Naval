#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>

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
    int tiempoJugado;
} Partida;
// DATOS DE LA PARTIDA

typedef struct nodo
{
    char nombre[MAX_NOMBRE];
    int disparos; //utilizados ppara ganar
    struct nodo *siguiente;
} Nodo;
//RANKING

/* funciones */
int  mostrarMenu(void);                 //0- origen
void Primera(Nodo** ranking);           //1- Nueva partida
void Segunda(Nodo** ranking);           //2- Cargar partida
void Tercera(void);                     //3- Instrucciones
void Cuarta(Nodo* ranking);            //4- Ranking;
void Quinta(void);                      //5- Salir;


//Nueva partida
void inicializarTableros(Partida* partida);
void mostrarTablero(char tablero[FILAS][COLUMNAS], int colores);
void GODMODE(Partida* partida); //testing y cheats
int disparar(Partida* partida, int fila, int columna);
void mostrarResultado(Partida* partida);

//Pausar partida
int guardarPartida(const Partida* partida);
int pausarPartida(Partida* partida);
void jugarPartida(Partida* partida, Nodo** ranking);


//Ranking
int insertarRanking (Nodo** ranking, char* nombre, int disparos);
int cargarRanking   (Nodo** ranking);
int guardarRanking  (Nodo* ranking);
void liberarRanking (Nodo** ranking);

//Funciones de posicions
int zonaLibre(int tablero[FILAS][COLUMNAS], int fila, int columna); //punto aleatoreo libre alrededor
void colocarFragatas(Partida* partida); //genero barco? lo guardo:falla inento en todas las posiciones(sino vuelvo atras
void Puntoaleatorio(Barco* barco);
void Direccionesaleatorias(int direcciones[4]);
int barcoLibre(Partida* partida, Barco* barco);
void colocarBarco(Partida* nueva, int indice, int tamanio);


int main(void)
{
    int entry = 0;
    int caracter;
    Nodo* ranking = NULL;

    srand((unsigned int)time(NULL)); //semilla random

    if (cargarRanking(&ranking) == 0) {
        printf("No se pudo cargar ranking.txt. Revise el archivo antes de jugar.\n");
        entry = 5;
    }

    while (entry != 5)
    {
        entry = mostrarMenu();
        system("cls");

        switch (entry)
        {
        case 1:
            Primera(&ranking);
            break;
        case 2:
            Segunda(&ranking);
            break;
        case 3:
            Tercera();
            break;
        case 4:
            Cuarta(ranking);
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
    }

    liberarRanking(&ranking);
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

void Primera(Nodo** ranking)
{
    /* Variables Locales*/
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
        colocarFragatas(&nueva);
        jugarPartida(&nueva, ranking);
        break;

    case EOF:
        printf("Se termino la entrada.\n");
        break;
    }
}

void mostrarResultado(Partida* partida)
{
    int minutos, segundos, puntos, bonusDisparos, bonusTiempo;
  
    minutos = partida->tiempoJugado / 60;
    segundos = partida->tiempoJugado % 60;
    
    //ecu. de puntos
    bonusDisparos = partida->disparosRestantes * 100;
    bonusTiempo = 60000 / (60 + partida->tiempoJugado);
    puntos = 1000 + bonusDisparos + bonusTiempo;

    printf("\n========== PARTIDA GANADA ==========\n");
    printf("Jugador: %s\n", partida->nombre);
    printf("Disparos utilizados: %d\n", partida->disparosRealizados);
    printf("Tiempo: %02d:%02d\n", minutos, segundos);
    printf("Puntos: %d", puntos);
    printf("\n====================================\n");
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

void mostrarTablero(char tablero[FILAS][COLUMNAS], int colores)
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
        //fila LETRAS:
        printf(" %c |", 'A' + fila); //Sumo a "A" para pasar de letras A-J
        for (columna = 0; columna < COLUMNAS; columna++)
        {
            if (colores == 1)//Principal
            {
                switch (tablero[fila][columna])
                {
                case 'B':               //Godmode: barco
                    printf("\033[1;97m");
                    break;
                case 'X':               //Godmode: barco hundido
                case 'T':               //algo!!!
                    printf("\033[91m"); 
                    break;
                case '*':               //con y sin godmode: Mar, equivocado, no le dio al blanco
                    printf("\033[93m");
                    break;
                    case 'H':           //Barco hundido
                    printf("\033[1;91m");
                    break;
                default:
                    printf("\033[94m"); // con y sin godmode: azul, O vacio
                    break;
                }
            }

            printf(" %c", tablero[fila][columna]);
            if (colores == 1) {
                printf("\033[0m");
            }
            printf(" |");
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
        for (columnaVecina = columna - 1; columnaVecina <= columna + 1; columnaVecina++)
        {
            if (filaVecina >= 0 && filaVecina < FILAS && columnaVecina >= 0 && columnaVecina < COLUMNAS)
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

            if (partida->tableroPosicion[fila][columna] != 0 && vista[fila][columna] == 'O')
            {
                vista[fila][columna] = 'B';
            }
        }
    }

    printf("\nGODMODE - Partida invalida\n");
    mostrarTablero(vista,1);
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
    int cambioFila[4] =     { -1, 1, 0, 0 };
    int cambioColumna[4] =  {  0, 0,-1, 1 };
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
    int cambioFila[4] =         { -1, 1,  0, 0 };
    int cambioColumna[4] =      {  0, 0, -1, 1 };
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

void inline Puntoaleatorio(Barco* barco) {
        barco->fila = rand() % FILAS;
        barco->columna = rand() % COLUMNAS;
    
}

void inline Direccionesaleatorias(int direcciones[4]) {
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

int disparar(Partida* partida, int fila, int columna){
    int realizado = 0 ;
    int indice, casilla, f, c;
    Barco* barco;
    if (fila >= 0 && fila < FILAS && columna >= 0 && columna < COLUMNAS &&
        partida->disparosRestantes > 0 && partida->barcosHundidos < Cant_Barcos
        && partida->tableroPrincipal[fila][columna] == 'O')
    {
        partida->disparosRealizados++;
        partida->disparosRestantes--;

        if (partida->tableroPosicion[fila][columna] == 0) { //al AGUA
            partida->tableroPrincipal[fila][columna] = '*';
        }
        else {
            partida->tableroPrincipal[fila][columna] = 'X';
            indice = partida->tableroPosicion[fila][columna] - 1;//real
            barco = &partida->barcos[indice];
            barco->impactos++;
            if (barco->impactos == barco->tamanio) {
                partida->barcosHundidos++;
                for (casilla = 0; casilla < barco->tamanio; casilla++) {
                    f = barco->fila + casilla * barco->direccionFila;
                    c = barco->columna + casilla * barco->direccionColumna;
                    partida->tableroPrincipal[f][c] = 'H';
                }
            }
        }
        realizado = 1;
    }
    return realizado;
}
void Segunda(Nodo** ranking)
{
    FILE* arch;
    Partida recuperada = { 0 };
    int leida;

    arch = fopen("partida.dat", "rb");
    if (arch == NULL) {
        printf("No hay una partida guardada disponible.\n");
    }
    else {
        leida = fread(&recuperada, sizeof(Partida), 1, arch) == 1;
        if (fclose(arch) != 0) {
            leida = 0;
        }
        if (leida == 0 || memchr(recuperada.nombre, '\0', MAX_NOMBRE) == NULL) {
            printf("No se pudo leer una partida completa.\n");
        }
        else if (recuperada.disparosRestantes <= 0 || recuperada.barcosHundidos >= Cant_Barcos) {
            printf("Esta partida termino. Guarda una nueva para reemplazarla.\n");
        }
        else {
            jugarPartida(&recuperada, ranking);
        }
    }
}

int guardarPartida(const Partida* partida)
{
    FILE* arch;
    int guardada = 0;

    arch = fopen("partida.dat", "wb");

    if (arch == NULL) {
        printf("No se pudo crear partida.dat.\n");
    }
    else {
        guardada = fwrite(partida, sizeof(Partida), 1, arch) == 1;

        if (fclose(arch) != 0) {
            guardada = 0;
        }

        if (guardada == 0) {
            printf("No se pudo completar el guardado.\n");
        }
    }

    return guardada;
}

void jugarPartida(Partida* partida, Nodo** ranking)
{
    int caracter, lectura, columna, fila;
    int godmode = 0; //1 cheats o 0 normal TESTING
    int continuar = 1;
    int comandoInvalido = 0;
    int guardada = 0;
    char comando[30];
    char letra, sobrante;
    int tiempoAnterior = partida->tiempoJugado;
    time_t inicio = time(NULL);

    do
    {
        system("cls");
        printf("Jugador: %s\n", partida->nombre);
        printf("Disparos restantes: %d\n", partida->disparosRestantes);
        printf("Barcos hundidos: %d / %d\n", partida->barcosHundidos, Cant_Barcos);

        if (godmode == 1) {
            GODMODE(partida);
        }
        else {
            mostrarTablero(partida->tableroPrincipal, 1);
        }

        if (comandoInvalido == 1) {
            printf("\nEntrada invalida o casilla ya utilizada.\n");
        }
        comandoInvalido = 0;

        printf("\ngodmode_1 / godmode_0 / volver\n");   //TESTING
        printf("Ingrese coordenada (ej. A5) o comando: ");

        if (fgets(comando, sizeof(comando), stdin) == NULL) {
            continuar = 0;
        }
        else if (strchr(comando, '\n') == NULL && feof(stdin) == 0) {
            while ((caracter = getchar()) != '\n' && caracter != EOF) {
            }
            comandoInvalido = 1;
        }
        else {
            comando[strcspn(comando, "\n")] = '\0';

            //PAUSA
            if (comando[0] == '\0') {
                partida->tiempoJugado = tiempoAnterior + (int)difftime(time(NULL), inicio);
                guardada = pausarPartida(partida);
                if (guardada == 1) {
                    continuar = 0;
                }
            }
            else if (strcmp(comando, "godmode_1") == 0) {
                godmode = 1;
            }
            else if (strcmp(comando, "godmode_0") == 0) {
                godmode = 0;
            }
            else if (strcmp(comando, "volver") == 0) {
                continuar = 0;
            }
            else {
                lectura = sscanf(comando, " %c %2d %c", &letra, &columna, &sobrante);
                if (lectura == 2) {
                    fila = toupper((unsigned char)letra) - 'A';
                    columna--;
                    comandoInvalido = disparar(partida, fila, columna) == 0;
                }
                else {
                    comandoInvalido = 1;
                }
            }
        }
    } while (continuar == 1 && partida->disparosRestantes > 0 &&
             partida->barcosHundidos < Cant_Barcos);

    partida->tiempoJugado = tiempoAnterior + (int)difftime(time(NULL), inicio);
    system("cls");
    printf("Jugador: %s\n", partida->nombre);
    printf("Disparos restantes: %d\n", partida->disparosRestantes);
    printf("Barcos hundidos: %d / %d\n", partida->barcosHundidos, Cant_Barcos);
    mostrarTablero(partida->tableroPrincipal, 1);

    if (guardada == 1) {
        printf("\nPartida guardada en partida.dat.\n");
    }
    if (partida->disparosRestantes == 0 && partida->barcosHundidos < Cant_Barcos) {
        printf("\nTe quedaste sin disparos. Fin de la partida.\n");
    }
    if (partida->barcosHundidos == Cant_Barcos) {
        mostrarResultado(partida);
        if (insertarRanking(ranking, partida->nombre, partida->disparosRealizados) == 1) {
            if (guardarRanking(*ranking) == 1) {
                printf("\nResultado guardado en ranking.txt.\n");
            }
            else {
                printf("\nNo se pudo guardar ranking.txt. El resultado sigue en memoria.\n");
            }
        }
    }

    // Guarda el estado final para impedir que se cargue una partida terminada.
    if (partida->disparosRestantes == 0 || partida->barcosHundidos == Cant_Barcos) {
        guardarPartida(partida);
    }
}

int pausarPartida(Partida* partida)
{
    char respuesta[30];
    char opcion, sobrante;
    int lectura, caracter;
    int respuestaValida = 0;
    int guardada = 0;

    while (respuestaValida == 0)
    {
        printf("\nGuardar y volver al menu? S / N para continuar: ");

        if (fgets(respuesta, sizeof(respuesta), stdin) == NULL) {
            respuestaValida = 1;
        }
        else if (strchr(respuesta, '\n') == NULL && feof(stdin) == 0) {
            while ((caracter = getchar()) != '\n' && caracter != EOF) {
            }
            printf("Respuesta incorrecta.\n");
        }
        else {
            lectura = sscanf(respuesta, " %c %c", &opcion, &sobrante);
            if (lectura == 1 && toupper((unsigned char)opcion) == 'S') {
                guardada = guardarPartida(partida);
                respuestaValida = guardada;
            }
            else if (lectura == 1 && toupper((unsigned char)opcion) == 'N') {
                respuestaValida = 1;
            }
            else {
                printf("Ingrese solamente S o N.\n");
            }
        }
    }

    return guardada;
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

void Cuarta(Nodo* ranking)
{
    Nodo* actual = ranking;
    int puesto = 1;

    printf("RANKING\n\n");

    if (actual == NULL) {
        printf("Todavia no hay partidas ganadas.\n");
    }
    else {
        printf("%-8s %-49s %s\n", "Puesto", "Jugador", "Disparos");

        while (actual != NULL) {
            printf("%-8d %-49s %d\n",
                puesto, actual->nombre, actual->disparos);
            actual = actual->siguiente;
            puesto++;
        }
    }
}

int insertarRanking(Nodo** ranking, char* nombre, int disparos)
{
    Nodo* actual = *ranking;
    Nodo* anterior = NULL;
    Nodo* nuevo;
    int insertado = 0;

    nuevo = malloc(sizeof(Nodo));
    if (nuevo != NULL) {
        while (actual != NULL && actual->disparos <= disparos) {
            anterior = actual;
            actual = actual->siguiente;
        }

        strcpy(nuevo->nombre, nombre);
        nuevo->disparos = disparos;
        nuevo->siguiente = actual;

        if (anterior != NULL) {
            anterior->siguiente = nuevo;
        }
        else {
            *ranking = nuevo;
        }
        insertado = 1;
    }
    else {
        printf("\nNo hay memoria para agregar el resultado.\n");
    }

    return insertado;
}

int cargarRanking(Nodo** ranking)
{
    FILE* arch;
    Nodo* recuperado = NULL;
    char linea[100];
    char nombre[MAX_NOMBRE];
    char sobrante;
    int disparos;
    int lectura;
    int cargado = 1;

    arch = fopen("ranking.txt", "r");
    if (arch == NULL) {
        if (errno != ENOENT) {
            cargado = 0;
        }
    }
    else {
        while (feof(arch) == 0 && ferror(arch) == 0 && cargado == 1) {
            if (fgets(linea, sizeof(linea), arch) != NULL) {
                lectura = sscanf(linea, " %49s %3d %c", nombre, &disparos, &sobrante);
                if (lectura == 2 && disparos > 0 && disparos <= MAX_DISPAROS) {
                    cargado = insertarRanking(&recuperado, nombre, disparos);
                }
                else {
                    cargado = 0;
                }
            }
        }

        if (ferror(arch) != 0) {
            cargado = 0;
        }
        if (fclose(arch) != 0) {
            cargado = 0;
        }

        // Reemplaza la lista solamente si pudo leer todos los resultados.
        if (cargado == 1) {
            liberarRanking(ranking);
            *ranking = recuperado;
        }
        else {
            liberarRanking(&recuperado);
        }
    }

    return cargado;
}

int guardarRanking(Nodo* ranking)
{
    FILE* arch;
    Nodo* actual = ranking;
    int guardado = 0;

    arch = fopen("ranking.txt", "w");
    if (arch != NULL) {
        guardado = 1;
        while (actual != NULL && guardado == 1) {
            if (fprintf(arch, "%s %d\n", actual->nombre, actual->disparos) < 0) {
                guardado = 0;
            }
            actual = actual->siguiente;
        }
        if (fclose(arch) != 0) {
            guardado = 0;
        }
    }

    return guardado;
}

void liberarRanking(Nodo** ranking)
{
    Nodo* actual;

    while (*ranking != NULL) {
        actual = *ranking;
        *ranking = (*ranking)->siguiente;
        free(actual);
    }
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

