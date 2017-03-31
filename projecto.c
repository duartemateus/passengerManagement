#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOME 50
#define MAX_BARCOS 5
#define MIN_NUMERO_BARCO 1
#define MAX_NUMERO_BARCO 1000
#define MIN_LOTACAO 1
#define MAX_LOTACAO 1000
#define MIN_PRECO 0
#define MAX_PRECO 200
#define MIN_ANO 1950
#define MAX_ANO 2013
#define MAX_HORA 23
#define MAX_MINUTO 59

typedef struct
{
    int dia, mes, ano;
} tData;

typedef struct
{
    int hora,minuto;
} tHora;

typedef struct
{
    int identificacao;
    char nome[MAX_NOME];
    tData dataInauguracao;
    tHora horaPartida;
    int estadoDaViagem; //0 se a viagem ainda nao foi realizada, 1 se a viagem estiver a ser realizada, 2 se a viagem estiver terminada
    int barco;
    float preco;
    int lotacaoMaxima;
    int passageiros; // quantidade de passageiros atualmente
} tViagem;

typedef struct
{
    int numero;
    char nome[MAX_NOME];
    tData dataInauguracao;
    int lotacaoMaxima;
    float preco;
    int associado; //0 se o barco estiver disponivel ou 1 caso nao esteja
} tBarco;

/********************** Funcoes dos menus **************************/
int menuPrincipal(int contadorBarcos,int contadorViagens);
int menuBarcos(void);
int menuViagem(void);
int menuFicheiros(void);

/********************** Funcoes auxiliares *************************/
void lerString(char str[], int maximoCaracteres);
void limparBufferTeclado(void);
float lerFloat(char mensagem[], int minimo, int maximo);
int lerInteiro(char mensagem[], int minimo, int maximo);
void lerData(tData *data);
int validarData(tData *data);
char resposta(void);

/********************** Funcoes dos Barcos *************************/
void inserirBarco(tBarco barcos[], int *contadorBarcos);
void alterarBarco(tBarco barcos[], int contadorBarcos);
void consultarBarcos(tBarco barcos[], int contadorBarcos);
void eliminarBarco(tBarco barcos[], int *contadorBarcos);
int procurarBarcoPorNumero(tBarco barcos[], int contadorBarcos, int numeroAProcurar);

/********************** Funcoes das Viagens ************************/
void inserirViagem(tViagem* ,tBarco barcos[], int *contadorViagens,int *barcosAssociados,int contadorBarcos);
void consultarViagem(tViagem* , int contadorViagens);
void alterarViagem(tViagem* viagem,tBarco barcos[], int contadorViagens, int barcosAssociados, int contadorBarcos);
/********************** Funcoes dos Ficheiros **********************/
void guardarBarcosFicheiroBinario(tBarco barcos[], int contadorBarcos, int barcosAssociados);
void lerBarcosFicheiroBinario(tBarco barcos[], int *contadorBarcos, int *barcosAssociados);
void guardarViagemFicheiroBinario(tViagem* , int contadorViagens);
void lerViagensFicheiroBinario(tViagem* , int *contadorViagens);

int main(void)
{
    tBarco barcos[MAX_BARCOS];// Estrutura de dados (vetor estatico) para armazenar informacao dos barcos
    tViagem* viagem;//estrutura de dados (dinamica) para armazenar informacao dos bacos

    int contadorBarcos = 0, contadorViagens = 0, barcosEmViagem = 0,barcosAssociados = 0;  // contador de barcos e viagens armazenados na estrutura de dados (vetor estatico)
    viagem = (tViagem*)malloc(sizeof(tViagem)+contadorViagens);
    if (viagem == 0)
    {
        printf("Erro a alocar memoria\n");
        return 1;
    }
    int opcao, opcaoBarcos,opcaoViagem, opcaoFicheiros;
    do
    {
        opcao = menuPrincipal(contadorBarcos, contadorViagens);
        switch(opcao)
        {
        case 1:
            do
            {
                opcaoBarcos = menuBarcos();
                switch(opcaoBarcos)
                {
                case 1:
                    inserirBarco(barcos, &contadorBarcos);
                    break;
                case 2:
                    alterarBarco(barcos, contadorBarcos);
                    break;
                case 3:
                    consultarBarcos(barcos, contadorBarcos);
                    break;
                case 4:
                    eliminarBarco(barcos, &contadorBarcos);
                    break;
                }
            }
            while (opcaoBarcos != 0);

            break;
        case 2:
            do
            {
                opcaoViagem = menuViagem();
                switch(opcaoViagem)
                {
                case 1:
                    viagem = realloc(viagem,sizeof(tViagem)+1);
                    inserirViagem(viagem, barcos,&contadorViagens,&barcosAssociados, contadorBarcos);
                    break;
                case 2:
                    alterarViagem(viagem,barcos, contadorViagens, barcosAssociados, contadorBarcos);
                    break;
                case 3:
                    consultarViagem(viagem, contadorViagens);
                    break;
                case 4:
                    //estadoViagem(viagem, contadorViagens);
                    break;
                }
            }
            while(opcaoViagem != 0);
            break;
        case 3:
            break;
        case 4:
            do
            {
                opcaoFicheiros = menuFicheiros();
                switch(opcaoFicheiros)
                {
                case 1:
                    guardarBarcosFicheiroBinario(barcos, contadorBarcos, barcosAssociados);
                    break;
                case 2:
                    lerBarcosFicheiroBinario(barcos, &contadorBarcos, &barcosAssociados);
                    break;
                case 3:
                    guardarViagemFicheiroBinario(viagem, contadorViagens);
                    break;
                case 4:
                    lerViagensFicheiroBinario(viagem, &contadorViagens);
                    break;
                }
            }
            while(opcaoFicheiros != 0);
            break;
        }
    }
    while (opcao != 0);
    free(viagem);
    return 0;
}

void limparBufferTeclado(void)
{
    char chr;
    do
    {
        chr = getchar();
    }
    while (chr != '\n' && chr != EOF);
}

float lerFloat(char mensagem[], int minimo, int maximo)
{
    float numero;
    int controlo;

    do
    {
        printf("%s (%d e %d): ", mensagem, minimo, maximo);
        controlo = scanf ("%f", &numero);
        limparBufferTeclado();
        if (numero < minimo || numero > maximo)
        {
            printf("O numero tem de ser entre %d e %d\n", minimo, maximo);
        }
    }
    while(numero<minimo || numero>maximo || controlo == 0);
    return numero;
}

int lerInteiro(char mensagem[], int minimo, int maximo)
{
    int numero, controlo;

    do
    {
        printf("%s (%d e %d): ", mensagem, minimo, maximo);
        controlo = scanf ("%d", &numero);
        limparBufferTeclado();
        if (numero < minimo || numero > maximo)
        {
            printf("O numero tem de ser entre %d e %d\n", minimo, maximo);
        }
    }
    while(numero<minimo || numero>maximo || controlo == 0);
    return numero;
}

void lerString(char str[], int maximoCaracteres)
{
    char *ptr = NULL;

    fgets(str, maximoCaracteres, stdin);
    ptr = strchr(str, '\n');
    if (ptr != NULL)
    {
        *ptr = '\0';
    }
}

int menuPrincipal(int contadorBarcos, int contadorViagens)
{
    int opcao;

    printf("Menu Principal\n\n");
    printf("Barcos Inseridos: %d\nViagens Realizadas: %d\n", contadorBarcos,contadorViagens);
    printf("1 - Barcos\n");
    printf("2 - Viagens\n");
    printf("3 - Estatisticas\n");
    printf("4 - Ficheiros\n");
    printf("\n0 - Sair\n");

    opcao = lerInteiro("\n\nOpcao -> ",0,4);

    return opcao;
}

int menuBarcos(void)
{
    int opcao;

    printf("Menu Barcos\n\n");
    printf("1 - Inserir Barco\n");
    printf("2 - Alterar Barco\n");
    printf("3 - Consultar Barcos\n");
    printf("4 - Eliminar Barco\n");
    printf("\n0 - Voltar ao Menu Principal\n");

    opcao = lerInteiro("\n\nOpcao -> ",0,4);

    return opcao;
}

int menuViagem(void)
{
    int opcao;

    printf("Menu Viagem\n\n");
    printf("1 - Inserir Viagem\n");
    printf("2 - Alterar Viagem\n");
    printf("3 - Consultar Viagem\n");
    printf("4 - Estado da Viagem\n");
    printf("\n0 - Voltar ao Menu Principal\n");
    opcao = lerInteiro("\n\nOpcao -> ",0,4);
    return opcao;
}

int menuFicheiros(void)
{
    int opcao;

    printf("Menu Ficheiros\n\n");
    printf("1 - Guardar Barcos para Ficheiro Binario\n");
    printf("2 - Ler Barcos de Ficheiro Binario\n");
    printf("3 - Guardar Viagens para Ficheiro Binario\n");
    printf("4 - Ler Viagens de Ficheiro Binario\n");
    printf("\n0 - Voltar ao Menu Principal\n");

    opcao = lerInteiro("\n\nOpcao -> ",0,4);

    return opcao;
}
void inserirViagem(tViagem* viagem,tBarco barcos[], int *contadorViagens, int *barcosAssociados, int contadorBarcos)
{
    int posicao, numeroAux, dataValida;
    if (contadorBarcos == 0)
    {
        printf("\nNão existem barcos.\n");
    }
    else
    {
        if ((contadorBarcos - (*barcosAssociados)) != 0)
        {
            printf("\nViagens Existentes:\n");
            consultarViagem(viagem, *contadorViagens);
            numeroAux = *contadorViagens+1;
            printf("Viagem Numero - %d\n", numeroAux);
            viagem[*contadorViagens].identificacao = numeroAux;
            printf("Designação da viagem: ");
            lerString(viagem[*contadorViagens].nome, MAX_NOME);
            do
            {
                printf("Data da Viagem\n");
                lerData(&viagem[*contadorViagens].dataInauguracao);
                dataValida = validarData(&viagem[*contadorViagens].dataInauguracao);
                if (dataValida == 0)
                {
                    printf("A data nao e valida! Insira uma data valida.\n");
                }
            }
            while(dataValida == 0);
            viagem[*contadorViagens].horaPartida.hora = lerInteiro("Hora de partida",0, MAX_HORA);
            viagem[*contadorViagens].horaPartida.minuto = lerInteiro("Minuto de partida",0, MAX_MINUTO);
            printf("\nBarcos Existentes:\n");
            consultarBarcos(barcos, contadorBarcos);
            do
            {
                viagem[*contadorViagens].barco = lerInteiro("Numero do Barco a realizar a viagem", MIN_NUMERO_BARCO, MAX_NUMERO_BARCO);
                posicao = procurarBarcoPorNumero(barcos, contadorBarcos, viagem[*contadorViagens].barco);
                if (posicao == -1)
                {
                    printf("Esse barco não existe, escolha outro.\n");
                }
                else
                {
                    if (barcos[posicao].associado == 1)
                    {
                        printf("Esse barco esta associado a uma viagem, escolha outro.\n");
                    }
                }
            }
            while(posicao == -1 || barcos[posicao].associado == 1);
            barcos[posicao].associado == 1;
            printf("\nBarco %d com o nome %s associado a viagem %d\n",barcos[posicao].numero, barcos[posicao].nome, viagem[*contadorViagens].identificacao);
            viagem[*contadorViagens].preco = barcos[posicao].preco;
            viagem[*contadorViagens].lotacaoMaxima = barcos[posicao].lotacaoMaxima;
            (*contadorViagens)++;
            (*barcosAssociados)++;
        }
        else
        {
            printf("\nNão existem barcos disponiveis\n");
        }
    }
}


void inserirBarco(tBarco barcos[], int *contadorBarcos)
{
    int posicao, numeroAux, dataValida;

    if (*contadorBarcos < MAX_BARCOS)
    {
        do
        {
            printf("\nBarcos Existentes:\n");
            consultarBarcos(barcos, *contadorBarcos);

            numeroAux = lerInteiro("Numero do Barco", MIN_NUMERO_BARCO, MAX_NUMERO_BARCO);
            posicao = procurarBarcoPorNumero(barcos, *contadorBarcos, numeroAux);
            if (posicao != -1)
            {
                printf("Esse numero ja existe! Escolha outro!\n");
            }
        }
        while(posicao != -1);

        barcos[*contadorBarcos].numero = numeroAux;
        printf("Nome do Barco: ");
        lerString(barcos[*contadorBarcos].nome, MAX_NOME);
        do
        {
            printf("Data de Inauguracao\n");
            lerData(&barcos[*contadorBarcos].dataInauguracao);
            dataValida = validarData(&barcos[*contadorBarcos].dataInauguracao);
            if (dataValida == 0)
            {
                printf("A data nao e valida! Insira uma data valida.\n");
            }
        }
        while(dataValida == 0);

        barcos[*contadorBarcos].lotacaoMaxima = lerInteiro("Lotacao Maxima do Barco", MIN_LOTACAO, MAX_LOTACAO);
        barcos[*contadorBarcos].preco = lerFloat("Preco por Lugar", MIN_PRECO, MAX_PRECO);
        (*contadorBarcos)++;
    }
    else
    {
        printf("Limite dos barcos atingido: %d\n Nao pode inserir mais barcos.\n", MAX_BARCOS);
    }
}
void alterarViagem(tViagem* viagem,tBarco barcos[], int contadorViagens, int barcosAssociados, int contadorBarcos)
{
    int numero, posicaoAlterar, existe, numeroAux, dataValida, limpar;
    char opcao;

    if (contadorViagens == 0)
    {
        printf("Nao existem barcos inseridos!\n");
    }
    else
    {
        printf("\nViagens Existentes:\n");
        consultarViagem(viagem, contadorViagens);
        numero = lerInteiro("Numero da viagem a alterar", 1, contadorViagens)-1; //viagem a ser alterada(indice)
        posicaoAlterar = procurarBarcoPorNumero(barcos, contadorBarcos, numero);
        if (viagem[numero].estadoDaViagem != 0)
        {
            printf("Essa viagem está a ser realizada por isso não pode ser alterada.\n");
        }
        else
        {
            printf("Deseja alterar o barco associado a esta viagem? ");
            opcao = resposta();
            if (opcao=='S')
            {
                if (contadorBarcos - barcosAssociados != 0)
                {
                do
                {
                    consultarBarcos(barcos, contadorBarcos);
                    numeroAux = lerInteiro("Numero do Barco a associar a viagem", MIN_NUMERO_BARCO, MAX_NUMERO_BARCO);
                    existe = procurarBarcoPorNumero(barcos, contadorBarcos, numeroAux);
                if (existe == -1)
                {
                    printf("Esse barco não existe, escolha outro.\n");
                }
                else
                {
                    if (barcos[existe].associado == 1)
                    {
                        printf("Esse barco já está associado a uma viagem, escolha outro.\n");
                    }
                }
                }
                while(existe == -1 || barcos[existe].associado == 1);
                limpar = procurarBarcoPorNumero(barcos, contadorBarcos, viagem[numero].barco);
                barcos[limpar].associado = 0;
                barcos[existe].associado = 1;
                viagem[numero].barco = barcos[existe].numero;
                viagem[numero].preco = barcos[existe].preco;
                viagem[numero].lotacaoMaxima = barcos[existe].lotacaoMaxima;
                }
                else{
                    printf("\nTodos os barcos estão associados a uma viagem\n");
                }
            }
            printf("Deseja alterar a desigancao?");
            opcao = resposta();
            if (opcao=='S')
            {
                printf("Designacao da viagem ");
                lerString(viagem[numero].nome, MAX_NOME);
            }

            printf("Deseja alterar a data da viagem? ");
            opcao = resposta();
            if (opcao=='S')
            {
                do
                {
                    printf("Data de Inauguracao\n");
                    lerData(&viagem[numero].dataInauguracao);
                    dataValida = validarData(&viagem[numero].dataInauguracao);
                    if (dataValida == 0)
                    {
                        printf("A data nao e valida! Insira uma data valida.\n");
                    }
                }
                while(dataValida == 0);
            }
           printf("Deseja alterar a hora da viagem? ");
            opcao = resposta();
            if (opcao=='S')
            {
                viagem[numero].horaPartida.hora = lerInteiro("Hora de partida",0, MAX_HORA);
                viagem[numero].horaPartida.minuto = lerInteiro("Minuto de partida",0, MAX_MINUTO);
            }
        }
    }
}
void alterarBarco(tBarco barcos[], int contadorBarcos)
{
    int numero, posicaoAlterar, existe, numeroAux, dataValida;
    char opcao;

    if (contadorBarcos == 0)
    {
        printf("Nao existem barcos inseridos!\n");
    }
    else
    {
        printf("\nBarcos Existentes:\n");
        consultarBarcos(barcos, contadorBarcos);
        numero = lerInteiro("Numero do barco a alterar", MIN_NUMERO_BARCO, MAX_NUMERO_BARCO);
        posicaoAlterar = procurarBarcoPorNumero(barcos, contadorBarcos, numero);
        if (posicaoAlterar == -1)
        {
            printf("Esse barco nao existe!\n");
        }
        else
        {
            printf("Deseja alterar o numero? ");
            opcao = resposta();
            if (opcao=='S')
            {
                do
                {
                    numeroAux = lerInteiro("Numero do Barco", MIN_NUMERO_BARCO, MAX_NUMERO_BARCO);
                    existe = procurarBarcoPorNumero(barcos, contadorBarcos, numeroAux);
                    if (existe != -1)
                    {
                        printf("Esse numero ja existe! Escolha outro!\n");
                    }
                }
                while(existe != -1);
                barcos[posicaoAlterar].numero = numeroAux;
            }

            printf("Deseja alterar o nome? ");
            opcao = resposta();
            if (opcao=='S')
            {
                printf("Nome do Barco: ");
                lerString(barcos[posicaoAlterar].nome, MAX_NOME);
            }

            printf("Deseja alterar a data de inauguracao? ");
            opcao = resposta();
            if (opcao=='S')
            {
                do
                {
                    printf("Data de Inauguracao\n");
                    lerData(&barcos[posicaoAlterar].dataInauguracao);
                    dataValida = validarData(&barcos[posicaoAlterar].dataInauguracao);
                    if (dataValida == 0)
                    {
                        printf("A data nao e valida! Insira uma data valida.\n");
                    }
                }
                while(dataValida == 0);
            }
            printf("Deseja alterar a lotacao maxima? ");
            opcao = resposta();
            if (opcao=='S')
            {
                barcos[posicaoAlterar].lotacaoMaxima = lerInteiro("Lotacao Maxima do Barco", MIN_LOTACAO, MAX_LOTACAO);
            }
            printf("Deseja alterar o preco? ");
            opcao = resposta();
            if (opcao=='S')
            {
                barcos[posicaoAlterar].preco = lerFloat("Preco por Lugar", MIN_PRECO, MAX_PRECO);
            }
        }
    }
}
void consultarViagem(tViagem* viagem, int contadorViagens)
{
    int i;
    if (contadorViagens == 0)
    {
        printf("Nao existem Viagens inseridas!\n\n");
    }
    else
    {
        for (i = 0 ; i < contadorViagens ; i++)
        {
            printf("\n Numero: %d\tNome: %s\nData de Partida: %d-%d-%d\n\tHora de Partida %d:%d\n\tLotacao Maxima: %d\n\tPreco:%.2f\n\n-------------------------------------------------------------------\n",
                   viagem[i].identificacao, viagem[i].nome, viagem[i].dataInauguracao.dia, viagem[i].dataInauguracao.mes, viagem[i].dataInauguracao.ano, viagem[i].horaPartida.hora, viagem[i].horaPartida.minuto,
                   viagem[i].lotacaoMaxima, viagem[i].preco);
        }
    }
}

void consultarViagemPorEstado(tViagem* viagem, int contadorViagens, int estadoViagem)
{
    int i;
        for (i = 0 ; i < contadorViagens ; i++)
        {
            if (viagem[i].estadoDaViagem = estadoViagem)
            {
                printf("\n Numero: %d\tNome: %s\nData de Partida: %d-%d-%d\n\tHora de Partida %d:%d\n\tLotacao Maxima: %d\n\tPreco:%.2f\n\n-------------------------------------------------------------------\n",
                   viagem[i].identificacao, viagem[i].nome, viagem[i].dataInauguracao.dia, viagem[i].dataInauguracao.mes, viagem[i].dataInauguracao.ano, viagem[i].horaPartida.hora, viagem[i].horaPartida.minuto,
                   viagem[i].lotacaoMaxima, viagem[i].preco);
            }
        }
}

void consultarBarcos(tBarco barcos[], int contadorBarcos)
{
    int i;
    if (contadorBarcos == 0)
    {
        printf("Nao existem barcos inseridos!\n\n");
    }
    else
    {
        for (i = 0 ; i < contadorBarcos ; i++)
        {
            printf("\nNumero: %d\tNome: %s\nData de Inauguracao: %d-%d-%d\n\tLotacao: %d\n\tPreco:%.2f\n\n-------------------------------------------------------------------\n",
                   barcos[i].numero, barcos[i].nome, barcos[i].dataInauguracao.dia,barcos[i].dataInauguracao.mes, barcos[i].dataInauguracao.ano,
                   barcos[i].lotacaoMaxima, barcos[i].preco);
        }
    }
}

void eliminarBarco(tBarco barcos[], int *contadorBarcos)
{
    int i, numero, posicao;

    if (*contadorBarcos == 0)
    {
        printf("Nao existem barcos inseridos!\n");
    }
    else
    {
        printf("\nBarcos Existentes:\n");
        consultarBarcos(barcos, *contadorBarcos);
        numero = lerInteiro("Numero do barco a eliminar", MIN_NUMERO_BARCO, MAX_NUMERO_BARCO);
        posicao = procurarBarcoPorNumero(barcos, *contadorBarcos, numero);
        if (posicao == -1)
        {
            printf("Esse barco nao existe!\n");
        }
        else
        {
            for (i = posicao ; i < *contadorBarcos-1 ; i++)
            {
                barcos[i] = barcos[i+1];
            }
            (*contadorBarcos)--;
            printf("Barco Eliminado!\n");
        }
    }
}

int procurarViagemPorEstado(tViagem* viagem, int contadorViagens, int estadoViagem)
{
    int i, posicao;

    posicao = -1;

    for(i = 0 ; i < contadorViagens ; i++)
    {
        if (viagem[i].estadoDaViagem == estadoViagem)
        {
            posicao=i;
            i=contadorViagens;
        }
    }
    return posicao;
}

int procurarBarcoPorNumero(tBarco barcos[], int contadorBarcos, int numeroAProcurar)
{
    int i, posicao;

    posicao = -1;

    for(i = 0 ; i < contadorBarcos ; i++)
    {
        if (barcos[i].numero == numeroAProcurar)
        {
            posicao=i;
            i=contadorBarcos;
        }
    }
    return posicao;
}

void lerData(tData *data)
{
    data->dia = lerInteiro("Dia", 1, 31);
    data->mes = lerInteiro("Mes", 1, 12);
    data->ano = lerInteiro("Ano", MIN_ANO, MAX_ANO);
}

void guardarViagemFicheiroBinario(tViagem* viagem, int contadorViagens)
{
    FILE * ficheiro;
    int numeroViagensEscritos;

    ficheiro = fopen("viagem.dat","wb");
    if (ficheiro == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fwrite(&contadorViagens, sizeof (int), 1, ficheiro);
        numeroViagensEscritos = fwrite(viagem, sizeof(tViagem), contadorViagens, ficheiro);
        if (numeroViagensEscritos < contadorViagens)
        {
            printf("Erro: foram guardados %d de %d viagens.\n", numeroViagensEscritos, contadorViagens);
        }
        fclose(ficheiro);
    }
}

void lerViagensFicheiroBinario(tViagem* viagem, int *contadorViagens)
{
    FILE * ficheiro;
    int numeroViagensLidas;

    ficheiro = fopen("viagem.dat","rb");
    if (ficheiro == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fread(contadorViagens, sizeof (int), 1, ficheiro);
        if (*contadorViagens > MAX_BARCOS)
        {
            printf("Serao apenas considerados os primeiros %d barcos!\n", MAX_BARCOS);
            *contadorViagens = MAX_BARCOS;
        }
        numeroViagensLidas = fread(viagem, sizeof(tViagem), *contadorViagens, ficheiro);
        if (numeroViagensLidas < *contadorViagens)
        {
            printf("Erro ao ler as viagens. Foram lidas %d viagens.\n", numeroViagensLidas);
            *contadorViagens = numeroViagensLidas;
        }
        fclose(ficheiro);
    }
}

void guardarBarcosFicheiroBinario(tBarco barcos[], int contadorBarcos, int barcosAssociados)
{
    FILE * ficheiro;
    int numeroBarcosEscritos;

    ficheiro = fopen("barcos.dat","wb");
    if (ficheiro == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fwrite(&contadorBarcos, sizeof (int), 1, ficheiro);
        fwrite(&barcosAssociados,sizeof (int),1,ficheiro);
        numeroBarcosEscritos = fwrite(barcos, sizeof(tBarco), contadorBarcos, ficheiro);
        if (numeroBarcosEscritos < contadorBarcos)
        {
            printf("Erro: foram guardados %d de %d barcos.\n", numeroBarcosEscritos, contadorBarcos);
        }
        fclose(ficheiro);
    }
}

void lerBarcosFicheiroBinario(tBarco barcos[], int *contadorBarcos, int *barcosAssociados)
{
    FILE * ficheiro;
    int numeroBarcosLidos;

    ficheiro = fopen("barcos.dat","rb");
    if (ficheiro == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fread(contadorBarcos, sizeof (int), 1, ficheiro);
        fread(barcosAssociados, sizeof (int), 1, ficheiro);
        if (*contadorBarcos > MAX_BARCOS)
        {
            printf("Serao apenas considerados os primeiros %d barcos!\n", MAX_BARCOS);
            *contadorBarcos = MAX_BARCOS;
        }
        numeroBarcosLidos = fread(barcos, sizeof(tBarco), *contadorBarcos, ficheiro);
        if (numeroBarcosLidos < *contadorBarcos)
        {
            printf("Erro ao ler os barcos. Foram lidos %d barcos.\n", numeroBarcosLidos);
            *contadorBarcos = numeroBarcosLidos;
        }
        fclose(ficheiro);
    }
}

int validarData(tData *data)
{
    int dias[]= {31,28,31,30,31,30,31,31,30,31,30,31};

    if ((data->ano % 400 == 0) || (data->ano % 4 == 0 && data->ano % 100 != 0))
    {
        dias[1] = 29;
    }

    if (data->dia <= dias[data->mes-1])
        return 1;

    return 0;
}

char resposta(void)
{
    char resposta;

    scanf(" %c",&resposta);
    limparBufferTeclado();
    resposta = toupper(resposta);

    return resposta;
}
