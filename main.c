/**
 ------------------------------------------------------------
    BIBLIOTECAS
 ------------------------------------------------------------
**/

#include <stdio.h>
#include <stdlib.h>

/**
 ------------------------------------------------------------
    MACROS
 ------------------------------------------------------------
**/

// N�mero de lados no retangulo
#define LADOS           3

// N�mero de elementos no jogo
#define MAX_ELEM        (LADOS*LADOS)

// N�mero de pe�as no jogo
#define PECAS           (MAX_ELEM-1)

// Setas do teclado
#define ARROW_UP        584     // arrow up
#define ARROW_DOWM      592     // arrow down
#define ARROW_RIGHT     589     // arrow right
#define ARROW_LEFT      587     // arrow left

/**
 ------------------------------------------------------------
    PROT�TIPO DE FUN��ES
 ------------------------------------------------------------
**/

void LimpaTela(void);
void Imprime(void);
void Jogo_Init(void);

/**
 ------------------------------------------------------------
    VARI�VEIS GLOBAIS
 ------------------------------------------------------------
**/

// Estrutura de dados utilizada para salvar informa��es do jogo
unsigned int jogo[MAX_ELEM];

/**
 ------------------------------------------------------------
    FUN��ES
 ------------------------------------------------------------
**/

// Limpa a tela e imprime cabe�alho
void LimpaTela(void)
{
    system("cls");

    // Cabe�alho exibido na primeira linha da tela
    printf(" Jogo do Oito:\n");
}

// Imprime o passo atual do jogo na tela
void Imprime(void)
{
    // Vari�veis locais
    unsigned int i, j;

    // Limpa a Tela
    LimpaTela();

    // Solicita ao jogador a pr�xima jogada
    printf(" Para jogar, utilize as setas do teclado!\n ");

    // Imprime matriz do jogo
    for (i = 0; i < MAX_ELEM; i++)
    {
        if ((i % LADOS) == 0)
        {
            if (!i) printf("\n ");
            else printf("|\n ");
#if (LADOS < 4)
            for (j = 0; j < LADOS; j++) printf("----");
#else
            for (j = 0; j < LADOS; j++) printf("-----");
#endif
            printf("-\n |");
        }
        else printf("|");

#if (LADOS < 4)
        if (jogo[i] == 0) printf("   ");
        else printf(" %d ", jogo[i]);
#else
        if (jogo[i] == 0) printf("    ");
        else printf(" %02d ", jogo[i]);
#endif
    }

    printf("|\n ");
#if (LADOS < 4)
    for (j = 0; j < LADOS; j++) printf("----");
#else
    for (j = 0; j < LADOS; j++) printf("-----");
#endif
    printf("-\n ");
}

// Inicializa a matriz do jogo com numeros aleatorios n�o repetidos
void Jogo_Init(void)
{
    // Vari�veis locais
    unsigned int i;
    unsigned int aux;
    unsigned int numbers[MAX_ELEM];

    // Inicializa indicador de algarismos j� usados
    for (i = 0; i < MAX_ELEM; i++)
        numbers[i] = 0;

    // Fornece nova semente ao gerador de numeros aleatorios
    srand((unsigned int)time(NULL));

    // Gera numeros aleatorios n�o repetidos
    for (i = 0; i < MAX_ELEM; i++)
    {
        aux = (rand() % MAX_ELEM);

        // testa se o numero gerado j� foi utilizado
        if (!numbers[aux])
        {
            jogo[i] = aux;
            numbers[aux] = 1;
        }
        else i--;
    }
}

int get_key()
{
    int c = getch(); //first getch() sees the first value
    switch (c)  //This will only take action if the value is 0 or 224
    {
    case 0:
        return (getch() + 256);  //and this second getch() eats the 2nd value
    case 224:
        return (getch() + 512); //and 256 or 512 is added to it and returned.
    }
    return c;   //if the value is not 0 or 224 (IE key is not special) normal value is returned
}

// Recebe uma jogada do usu�rio
void RecebeJogada(void)
{
    // Vari�veis locais
    unsigned int i;

    // Encontra posi��o vazia
    for (i = 0; i < MAX_ELEM; i++)
        if (!jogo[i]) break;

    // Ponto de retorno em caso de erro
    for (;;)
    {
        // Testa as setas do teclado
        switch(get_key())
        {
        case ARROW_UP:

            // Testa erros
            if (i >= (MAX_ELEM-LADOS)) printf("\n Este movimento e invalido!\n");
            else
            {
                // Efetua a troca de elementos
                jogo[i] = jogo[(i+LADOS)];
                jogo[(i+LADOS)] = 0;
                return;
            }
            break;
        case ARROW_DOWM:

            // Testa erros
            if (i < LADOS) printf("\n Este movimento e invalido!\n");
            else
            {
                // Efetua a troca de elementos
                jogo[i] = jogo[(i-LADOS)];
                jogo[(i-LADOS)] = 0;
                return;
            }
            break;
        case ARROW_RIGHT:

            // Testa erros
            if ((i%LADOS) == 0) printf("\n Este movimento e invalido!\n");
            else
            {
                // Efetua a troca de elementos
                jogo[i] = jogo[(i-1)];
                jogo[(i-1)] = 0;
                return;
            }
            break;
        case ARROW_LEFT:

            // Testa erros
            if ((i%LADOS) == (LADOS-1)) printf("\n Este movimento e invalido!\n");
            else
            {
                // Efetua a troca de elementos
                jogo[i] = jogo[(i+1)];
                jogo[(i+1)] = 0;
                return;
            }
            break;
        }
    }
}

// Testa se o jogo acabou
unsigned int Jogo_Continua(void)
{
    /*  Continua = 1
        Terminou = 0 */

    // Vari�veis locais
    unsigned int i;

    // Testa se todas as posi��es est�o em ordem, menos a ultima
    for (i = 0; i < (MAX_ELEM-1); i++)
        if (jogo[i] != (i+1)) return 1;

    // N�o terminou o jogo ainda
    return 0;
}

/**
 ------------------------------------------------------------
    MAIN
 ------------------------------------------------------------
**/

int main()
{
    // Vari�veis locais
    unsigned int jogada;
    char aux;

    do
    {
        // Inicializa as variaveis
        Jogo_Init();

        // Reatualiza a impress�o da matriz na tela para o usu�rio
        Imprime();

        do
        {
            // Solicita para que o usu�rio efetue uma jogada
            RecebeJogada();

            // Reatualiza a impress�o da matriz na tela para o usu�rio
            Imprime();

            // Incrementa contador de jogadas
            jogada++;
        }
        // Testa se o jogo terminou
        while (Jogo_Continua());

        // Exibe o resultado do jogo
        printf("\n\n Parabens! Voce concluiu o jogo!\n\n ");

        // Espera o usu�rio clicar uma tecla do teclado
        system("pause");

        // Limpa a Tela
        LimpaTela();

        // Pergunta se o usuario gostaria de jogar novamente
        printf("\n Voce gostaria de jogar novamente? (Y/N)\n\n ");
        scanf("%c", &aux);
        fflush(stdin);
        aux = toupper(aux);
    }
    while (aux == 'Y');

    // Encerra o programa
    return 0;
}

/**
 ------------------------------------------------------------
    FIM
 ------------------------------------------------------------
**/
