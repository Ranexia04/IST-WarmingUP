#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <string.h>
#include "file.h"

void reconhecimento_parametros(int argc, char *argv[], int *modo_grafico, int *modo_texto, int *modo_pais, int *modo_cidade)
{
    int i = 0;

    for(i=0; i<argc; i++)
    {
        if(strcmp(argv[i], "-g") == 0)//se encontrar -g ativa o modo grafico
        {
            *modo_grafico = 1;
            printf("Modo gráfico ativado!\n");
        }

        if(strcmp(argv[i], "-t") == 0)//se encontrar -t ativa o modo texto
        {
            *modo_texto = 1;
            printf("Modo texto ativado!\n");
        }

        if(strcmp(argv[i], "-f1") == 0)//se encontrar -f1 ativa o modo paises
        {
            *modo_pais = i;
            printf("Escolheu ler os dados do ficheiro ordenado por países!\n");
        }

        if(strcmp(argv[i], "-f2") == 0)//se encontrar -f2 ativa o modo cidades
        {
            *modo_cidade = i;
            printf("Escolheu ler os dados do ficheiro ordenado por cidades!\n");
        }
    }

    if(*modo_grafico == 1 && *modo_texto == 1)//prevenção de erros
    {
        printf("Não pode ativar simultâneamento o modo gráfico e o modo de texto!!!\n");
        exit(EXIT_FAILURE);
    }

    if((*modo_pais == -1 && *modo_cidade == -1) || (*modo_grafico == -1 && *modo_texto == -1))
    {
        printf("Necessita de introduzir os parâmentros corretos.\n");
        exit(EXIT_FAILURE);
    }
}

node_t *leitura_dados(int indice, char *argv[], int modo)
{
    int a=0;
    node_t *newNode = NULL, *head = NULL, *aux = NULL;
    FILE *fp = fopen(argv[indice + 1], "r");
    char buffer[MAX_CHAR], *apc = NULL;

    if(fp == NULL)
    {
        printf("Este ficheiro não existe. Saíndo do programa.\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(buffer, MAX_CHAR, fp) != NULL )
    {
        if(modo == 1)// modo país, alguns segundos para short, 1 hora para all
        {

            if(strstr(buffer, "dt,AverageTemperature") != NULL) continue;//se encontrar esta linha problematica passa a frente

            newNode = (node_t*)checkedMalloc(sizeof(node_t));

            //como sscanf pára quando há espaço, estes sao substituidos por pontos
            while((apc = strchr(buffer, ' ')) != NULL) *apc = '.';

            sscanf(buffer, "%d-%d-%d,%f,%f,%s", &newNode->payload.dt.ano, &newNode->payload.dt.mes, &newNode->payload.dt.dia,
            &newNode->payload.temperatura, &newNode->payload.incerteza, newNode->payload.pais);

            if(newNode->payload.temperatura == 0.0f)//a temperatura é exatemente 0 caso existir ",," no ficheiro, logo passa à frente.
            {
                free(newNode);
                continue;
            }

            //volta a colocar espaços nas strings do pais em vez de pontos
            while((apc = strchr(newNode->payload.pais, '.')) != NULL) *apc = ' ';

            //ordenação da lista dos países
            if(aux == NULL)//cria head
            {
                newNode->prev = NULL;
                newNode->next = NULL;
                aux = newNode;
                head = newNode;
            }

            else if(newNode->payload.dt.ano*10000+newNode->payload.dt.mes*100+newNode->payload.dt.dia >
            aux->payload.dt.ano*10000+aux->payload.dt.mes*100+aux->payload.dt.dia)
            {//este if serve para determinar o sentido do movimento do aux, neste caso move-se no sentido do next

                a=0;
                while(a==0)
                {
                    if(aux->next == NULL)// se o aux alcançar o final da lista coloca o node no final
                    {
                        newNode->prev = aux;
                        newNode->next = NULL;
                        aux->next = newNode;
                        aux = newNode;
                        a=1;
                    }
                    else if(newNode->payload.dt.ano*10000+newNode->payload.dt.mes*100+newNode->payload.dt.dia >
                    aux->next->payload.dt.ano*10000+aux->next->payload.dt.mes*100+aux->next->payload.dt.dia)
                    {//continua a movimentar-se no sentido do next
                        aux = aux->next;
                    }
                    else
                    {//encontrou o local onde o node deve ser colocado no meio da lista
                        aux->next->prev = newNode;
                        newNode->next = aux->next;
                        aux->next = newNode;
                        newNode->prev = aux;
                        a=1;
                    }
                }
            }
            else//neste caso move-se no sentido do prev
            {
                a=0;
                while(a==0)
                {
                    if(aux->prev == NULL)
                    {// se o aux alcançar o inicio da lista coloca o node no inicio e torna-o head
                        newNode->prev = NULL;
                        newNode->next = aux;
                        aux->prev = newNode;
                        aux = newNode;
                        a=1;
                        head = newNode;
                    }
                    else if(newNode->payload.dt.ano*10000+newNode->payload.dt.mes*100+newNode->payload.dt.dia <
                     aux->prev->payload.dt.ano*10000+aux->prev->payload.dt.mes*100+aux->prev->payload.dt.dia)
                    {//continua a movimentar-se no sentido do prev
                        aux = aux->prev;
                    }
                    else
                    {//encontrou o local onde o node deve ser colocado no meio da lista
                        aux->prev->next = newNode;
                        newNode->prev = aux->prev;
                        aux->prev = newNode;
                        newNode->next = aux;
                        a=1;
                    }
                }
            }
        }
        else if(modo == 2)// modo cidade, imediato para short e all
        {

            if(strstr(buffer, "dt,AverageTemperature") != NULL) continue;//se encontrar esta linha problematica passa a frente

            newNode = (node_t*)checkedMalloc(sizeof(node_t));

            while((apc = strchr(buffer, ',')) != NULL) *apc = '\t';//substituimos as virgulas por tab por dava problemas com o sscanf

            while((apc = strchr(buffer, ' ')) != NULL) *apc = '.';//como sscanf pára quando há espaço, estes sao substituidos por pontos

            sscanf(buffer, "%d-%d-%d\t%f\t%f\t%s\t%s\t%f%c\t%f%c", &newNode->payload.dt.ano, &newNode->payload.dt.mes, &newNode->payload.dt.dia,
             &newNode->payload.temperatura, &newNode->payload.incerteza,
            newNode->payload.cidade, newNode->payload.pais, &newNode->payload.latitude.angular, &newNode->payload.latitude.direcao,
             &newNode->payload.longitude.angular, &newNode->payload.longitude.direcao);

            if(newNode->payload.temperatura == 0.0f)//a temperatura é exatemente 0 caso existir ",," no ficheiro, logo passa à frente.
            {
                free(newNode);
                continue;
            }

            //prevenção de erro pois interpretava o E como exponencial
            if(newNode->payload.longitude.direcao!='W') newNode->payload.longitude.direcao='E';

            //volta a colocar espaços nas strings do pais e da cidade
            while((apc = strchr(newNode->payload.cidade, '.')) != NULL) *apc = ' ';
            while((apc = strchr(newNode->payload.pais, '.')) != NULL) *apc = ' ';

            if(aux == NULL)// cria head
            {
                newNode->prev = NULL;
                newNode->next = NULL;
                aux = newNode;
                head = newNode;
            }
            else// forma a lista de acordo com a ordem do ficheiro
            {
                newNode->prev = aux;
                newNode->next = NULL;
                aux->next = newNode;
                aux = newNode;
            }
        }

    }

    return head;
}

void *checkedMalloc(size_t sz)  // faz malloc e faz verificação; size_t = unsized int - inteiro sem sinal
{
    void *mem = malloc(sz);

    if ( mem == NULL )
    {
        printf ("Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    return mem;

}

node_t *copia_lista(node_t *head, node_t *contas_head)
{
    node_t *aux = head, *aux_contas = NULL, *contas_tail = NULL;

    //recebe uma lista, cria outra e copia os dados da primeira para a segunda
    while(aux != NULL)
    {
        aux_contas = (node_t*)checkedMalloc(sizeof(node_t));

        if(contas_head == NULL)
        {
            contas_head = aux_contas;
            contas_tail = aux_contas;
        }
        aux_contas->payload = aux->payload;
        aux_contas->payload.dt.ano = aux->payload.dt.ano;
        aux_contas->payload.dt.mes = aux->payload.dt.mes;
        aux_contas->payload.dt.dia = aux->payload.dt.dia;
        aux_contas->payload.temperatura = aux->payload.temperatura;
        aux_contas->payload.incerteza = aux->payload.incerteza;
        aux_contas->payload.longitude = aux->payload.longitude;
        aux_contas->payload.latitude  = aux->payload.latitude;
        strcpy(aux_contas->payload.pais, aux->payload.pais);
        strcpy(aux_contas->payload.cidade,aux->payload.cidade);


        aux_contas->next = NULL;
        aux_contas->prev = contas_tail;
        contas_tail->next = aux_contas;
        contas_tail = aux_contas;

        aux = aux->next;

    }

    return contas_head;
}

//n é o número que queremos retornar, min é o numero minimo que pode ser e max é o maximo
int verifica_num(int n, int min, int max)
{
    char buffer[8] = "";
    char *pch = NULL;

    while (n == 0)
    {
        buffer[0] = '\0';

        fgets(buffer, 7, stdin);

        //retira o /n e substitui por \0
        if((pch = strchr(buffer, '\n')) != NULL) *pch = '\0';

        if(sscanf(buffer, "%d", &n) != 1)//apenas aceita o valor se for um número
        {
            n = 0;
            printf("Formato de número inválido.\n");
            continue;
        }

        if(n < min || n > max)//tem de estar nos parametros introduzidos
        {
            n = 0;
            printf("O número tem de estar entre %d e %d.\n", min, max);
        }
    }

    return n;
}

void introduz_string(char regiao[])
{
    char *pch = NULL;

    printf("Introduza a região que quer estudar.\n");
    fgets(regiao, MAX_CHAR, stdin);

    //substitui \n por \0
    if((pch = strchr(regiao, '\n')) != NULL) *pch = '\0';
}

int verifica_regiao(node_t *head, int tipo, char regiao[])
{//verifica se a string itroduzida está no fichero pelo menos uma vez
    node_t *aux = head;
    int existe = 0;

    while(aux != NULL)
    {
        if(tipo == 1)//modo pais
        {
            if(strcmp(aux->payload.pais, regiao) == 0) existe = 1;
        }
        else if(tipo == 2)//modo cidade
        {
            if(strcmp(aux->payload.cidade, regiao) == 0) existe = 1;
        }
        aux = aux->next;
    }

    return existe;
}

void free_memory(node_t *head)//liberta a memória no caso de ser do tipo node_t
{
    node_t *aux = head;

    while(aux->next != NULL)
    {
        aux = aux->next;
        free(aux->prev);
    }
    free(aux);
}

void free_memory_2(n_analise *head)//liberta memória no caso de ser do tipo n_analise
{
    n_analise *aux = head;
    n_analise *aux_2 = NULL;

    while(aux->next != NULL)
    {
        aux_2 = aux;
        aux = aux->next;
        free(aux_2);
    }
    free(aux);
}
