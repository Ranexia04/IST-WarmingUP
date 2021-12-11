#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <string.h>
#include "file.h"

//Trabalho realizado por Henrique Lourenco 90092 e Rodrigo Aires 90182
int main(int argc, char *argv[])
{
    int ano_i = 3000, ano_f = 0;
    node_t *f1_head = NULL, *f2_head = NULL;
    int modo_grafico = -1, modo_texto = -1, modo_pais = -1, modo_cidade = -1;// == -1 se for inativo
    node_t *aux = NULL;
    float temp_min = 300.0f, temp_max = -300.0f;

    //interpreta os parametros introduzidos
    reconhecimento_parametros(argc, argv, &modo_grafico, &modo_texto, &modo_pais, &modo_cidade);

    //prevenção de erros
    if(modo_texto!= -1 && modo_grafico != -1)
    {
        printf("Não pode ativar simultâneamente o modo de texto e o modo gráfico!\n");
        exit(EXIT_FAILURE);
    }
    else if(modo_texto == -1 && modo_grafico == -1)
    {
        printf("Tem de ativar pelo menos o modo de texto ou o modo gráfico!\n");
        exit(EXIT_FAILURE);
    }

    if(strstr(argv[modo_pais+1], "cities") != NULL)
    {
        printf("Tem de introduzir o nome do ficheiro correto!\n");
        exit(EXIT_FAILURE);
    }

    if(strstr(argv[modo_cidade+1], "countries") != NULL)
    {
        printf("Tem de introduzir o nome do ficheiro correto!\n");
        exit(EXIT_FAILURE);
    }

    //le apenas os ficheiros de acordo com os parametros introduzidos
    if(modo_pais != -1) f1_head = leitura_dados(modo_pais, argv, 1);

    if(modo_cidade != -1) f2_head = leitura_dados(modo_cidade, argv, 2);

    if(f1_head == NULL && f2_head == NULL)
    {
        printf("Erro ao ler os ficheiros!!!");
        exit(EXIT_FAILURE);
    }

    if(modo_texto == 1) menu_texto(f1_head, f2_head);

    if(modo_grafico == 1)
    {
        if(modo_pais != -1) printf("Apesar do ficheiro dos países ter sido lido, este não será utilizado no modo gráfico!\n");
        aux = f2_head;
        while(aux != NULL)//calcula o ano inicial e o ano final para ser utilizado para a seleção de cores
        {
            if(aux->payload.dt.ano < ano_i) ano_i = aux->payload.dt.ano;
            if(aux->payload.dt.ano > ano_f) ano_f = aux->payload.dt.ano;
            if(aux->payload.temperatura < temp_min) temp_min = aux->payload.temperatura;
            if(aux->payload.temperatura >temp_max) temp_max = aux->payload.temperatura;
            aux = aux->next;
        }
        main_grafico(f2_head, ano_i, ano_f, temp_min, temp_max);
        if(f1_head != NULL) free_memory(f1_head);//liberta memória
        if(f2_head != NULL) free_memory(f2_head);
    }

    return EXIT_SUCCESS;
}

