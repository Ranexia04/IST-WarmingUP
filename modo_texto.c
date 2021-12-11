#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <string.h>
#include "file.h"

//função menu_texto
//recebe: head da lista com a informação do ficheiro dos paises
//        head da lista com a informação do ficheiro das cidades

void menu_texto(node_t* head_countries,node_t* head_cities)
{
    int opcao = 0; // guarda a opcao que o utilizador escolheu no menu do modo de texto
    node_t *contas_head_countries = NULL, *contas_head_cities = NULL; // heads das listas com os dados filtrados

    printf("Bem vindo ao menu do modo de texto.\n");

    while(1)
    {
        printf("Escolha uma das 4 opções seguintes:\n1 - Filtragem de dados.\n2 - Histórico de temperaturas.\n3 - Análise de temperatura por ano.\n4 - Análise da temperatura global.\n5 - Sair do programa.\n");
        opcao=0;
        opcao = verifica_num(opcao, 1, 5);

        switch(opcao)
        {
            case 1:
                filtragem_dados(head_countries, &contas_head_countries, head_cities, &contas_head_cities);
                break;

            case 2:
                if(contas_head_countries == NULL) contas_head_countries = head_countries; //caso não tenha ocorrido anteriormente a filtragem de dados os ponteiros contas_head_countries e contas_head_cities estão a null
                if(contas_head_cities == NULL) contas_head_cities = head_cities;          // e tem de apontar para as listas com os dados provenientes dos ficheiros
                historico_temp(contas_head_countries, contas_head_cities);
                break;

            case 3:
                if(contas_head_countries == NULL) contas_head_countries = head_countries;
                if(contas_head_cities == NULL) contas_head_cities = head_cities;

                analise_ano(contas_head_countries, contas_head_cities);
                break;

            case 4:
                if(contas_head_countries == NULL) contas_head_countries = head_countries;
                if(contas_head_cities == NULL) contas_head_cities = head_cities;
                analise_global(contas_head_countries, contas_head_cities);
                break;

            case 5:
                printf("A sair do programa.\n");

                if(contas_head_countries != head_countries)                               //contas_head_countries e contas_head_cities podem estar a apontar para a head countries e head cities respectivamente
                    if(contas_head_countries != NULL) free_memory(contas_head_countries); // nesssa situação não ocorre a função free para evitar seg_fault
                if(contas_head_cities != head_cities)
                    if(contas_head_cities != NULL) free_memory(contas_head_cities);

                if(head_countries != NULL) free_memory(head_countries);
                if(head_cities != NULL) free_memory(head_cities);
                return;

            default:
                printf("Erro. Número inválido.\n");
        }
    }
}

//recebe: head da lista com a informação do ficheiro dos paises
//        head da lista com a informação do ficheiro das cidades
//        ponteiro para head de uma lista
//        ponteira para head da uma lista
void filtragem_dados(node_t *head_countries, node_t **contas_head_countries,node_t *head_cities,node_t **contas_head_cities)
{
    int opcao = 0, mes = 0, ano = 0, mes_i =0 , mes_f = 0;

    printf("Bem vindo ao menu da Filtragem de Dados! ");

    while(1)
    {
        printf("Escolha uma das seguintes opções:\n1 - Limpa critérios.\n2 - Escolhe intervalo para análise.\n3 - Escolhe mês para análise.\n4 - Voltar ao menu inicial.\n");
        opcao = 0;
        opcao = verifica_num(opcao, 1, 4);

        switch(opcao)
        {
            case 1:
                if (head_countries != NULL) // se o ficheiro dos paises foi carregado
                {
                    if(*contas_head_countries != NULL) *contas_head_countries = apaga_lista(*contas_head_countries); // a lista é apagada se já existir
                    *contas_head_countries = copia_lista(head_countries, *contas_head_countries);
                }

                if (head_cities != NULL) // se o ficheiro  das cidades foi carregado
                {
                    if(*contas_head_cities != NULL) *contas_head_cities = apaga_lista(*contas_head_cities);// a lista é apagada se já existir
                    *contas_head_cities = copia_lista(head_cities, *contas_head_cities);
                }
                printf("Critérios limpos.\n");
                break;

            case 2:
                pede_criterios(&mes, &ano); // os parametros ano e mes são pedido ao utilizador e retornados

                if(head_countries != NULL)
                {
                    if(*contas_head_countries == NULL) *contas_head_countries = copia_lista(head_countries, *contas_head_countries);//se a lista contas_head_countries ainda não existe
                    *contas_head_countries = inter_analise_pais(*contas_head_countries, mes, ano);                                  //então a lista é criada sendo uma copia da lista head_countries
                }

                if(head_cities!= NULL)
                {
                    if(*contas_head_cities==NULL) *contas_head_cities = copia_lista(head_cities, *contas_head_cities);//se a lista contas_head_cities ainda não existe
                    *contas_head_cities = inter_analise_cidade(*contas_head_cities, mes, ano);                        //então a lista é criada sendo uma copia da lista head_cities
                }

                printf("Operação realizada com sucesso.\n");
                break;

            case 3:
                pede_mes(&mes_i,&mes_f);// os parametros mes inicial e mes final é pedido ao utilizador e retornado

                if(head_countries != NULL)
                {
                    if(*contas_head_countries == NULL) *contas_head_countries = copia_lista(head_countries, *contas_head_countries);
                    *contas_head_countries = mes_analise(*contas_head_countries, mes_i, mes_f);
                }
                if(head_cities != NULL)
                {
                    if(*contas_head_cities == NULL) *contas_head_cities = copia_lista(head_cities, *contas_head_cities);
                    *contas_head_cities = mes_analise(*contas_head_cities, mes_i, mes_f);
                }

                printf("Operação realizada com sucesso.\n");
                break;

            case 4:
                printf("Voltando ao menu inicial.\n");
                return;
                break;

            default:
                printf("Erro. Número inválido.\n");
        }
    }
}

//recebe dois ponteiros de interiros
void pede_criterios(int* mes, int* ano)
{
    *mes = 0;
    *ano = 0;

    printf("Introduza o ano a partir do qual quer estudar a temperatura. (Entre 1700 e 2020.)\n");
    *ano = verifica_num(*ano, 1700, 2020);

    printf("Introduza o mês do ano escolhido a partir do qual quer estudar a temperatura. Nota: 1 corresponde a janeiro tal como 12 corresponde a dezembro\n");
    *mes = verifica_num(*mes, 1, 12);
}

// função que elimina os nodes com data anterior ao ano e mes introduzidos pelo utilizador, para os dados do ficheiro dos paises
node_t *inter_analise_pais(node_t *contas_head, int mes, int ano)
{

    node_t *aux = contas_head;

    while((aux->payload.dt.ano*100 + aux->payload.dt.mes) < (ano*100 + mes)) // concatenação do ano com o mes
    {
        aux = aux->next;
        free(aux->prev);
    }
    contas_head = aux;
    contas_head->prev = NULL;

    return contas_head;
}
// função que elimina os nodes com data anterior ao ano e mes introduzidos pelo utilizador, para os dados do ficheiro das cidades
node_t *inter_analise_cidade(node_t *contas_head,int mes, int ano)

{
    node_t *aux=contas_head, *aux_elimina;

    while(aux->next != NULL) // para evitar segemtation fault o while termina quando o aux alcança o ultimo node da lista
    {
        if(aux->payload.dt.ano*100 + aux->payload.dt.mes < ano*100 + mes)
        {
            if(aux == contas_head)
            {
                aux = aux->next;
                contas_head = aux;
                free(aux->prev);
                aux->prev = NULL;
            }
            else
            {
                aux->prev->next = aux->next;
                aux->next->prev = aux->prev;
                aux_elimina = aux;
                aux = aux->next;
                free(aux_elimina);
            }
        }
        else
        {
            aux = aux->next;
        }
    }

    if(aux->payload.dt.ano*100 + aux->payload.dt.mes < ano*100 + mes ) //como o ultimo node não fui processado no while é processado aqui
    {
        aux->prev->next = NULL;
        free(aux);
    }

    return contas_head;
}

void pede_mes(int *mes_i, int *mes_f)
{
    printf("Introduza o mês a partir do qual quer estudar a temperatura. Nota: 1 conresponde a janeiro tal como 12 conresponde a dezembro\n");
    *mes_i = verifica_num(*mes_i, 1, 12);

    printf("Introduza o mês até ao qual quer estudar a temperatura. Nota: 1 conresponde a janeiro tal como 12 conresponde a dezembro\n");
    *mes_f = verifica_num(*mes_f, 1, 12);
}

// função que elimina os nodes que correspondem a meses que o utilizador pediu para eliminar
node_t *mes_analise(node_t *contas_head, int mes_i, int mes_f)
{
    node_t *aux = contas_head, *aux_elimina = NULL;

    if(mes_i <= mes_f)
    {
        while(aux->next != NULL)
        {
            if(aux->payload.dt.mes < mes_i || aux->payload.dt.mes > mes_f)
            {
                if(aux == contas_head)
                {
                    aux = aux->next;
                    contas_head = aux;
                    free(aux->prev);
                    aux->prev = NULL;
                }
                else
                {
                    aux->prev->next = aux->next;
                    aux->next->prev = aux->prev;
                    aux_elimina = aux;
                    aux = aux->next;
                    free(aux_elimina);
                }
            }
            else
            {
                aux = aux->next;
            }
        }

        if(aux->payload.dt.mes < mes_i || aux->payload.dt.mes > mes_f)
        {
            aux->prev->next = NULL;
            free(aux);
        }

    }
    else
    {
        while(aux->next != NULL)
        {
            if(aux->payload.dt.mes < mes_i && aux->payload.dt.mes > mes_f)
            {
                if(aux == contas_head)
                {
                    aux = aux->next;
                    contas_head = aux;
                    free(aux->prev);
                    aux->prev = NULL;
                }
                else
                {
                    aux->prev->next = aux->next;
                    aux->next->prev = aux->prev;
                    aux_elimina = aux;
                    aux = aux->next;
                    free(aux_elimina);
                }
            }
            else
            {
                aux = aux->next;
            }
        }

        if(aux->payload.dt.mes < mes_i && aux->payload.dt.mes > mes_f)
        {
            aux->prev->next = NULL;
            free(aux);
        }

    }

    return contas_head;
}

//função que recebe uma head e apaga a lista correspondente
node_t *apaga_lista(node_t *contas_head)
{
    node_t *aux = contas_head;

    while(aux->next != NULL) // para evitar segemtation fault o while termina quando o aux alcança o ultimo node da lista
    {
        aux = aux->next;
        free(aux->prev);
        aux->prev = NULL;
    }
    free(aux);          // ultimo node da lista é apagado aqui
    contas_head = NULL;

    return contas_head;
}

// funçao que realiza o historico de temperatura
void historico_temp(node_t *countries_head, node_t * cities_head)
{
    int opcao = 0, intervalo = 0, existe = 0;
    char regiao[MAX_CHAR] = "";

    printf("Qual é o intervalo de anos em que pretende ver os dados?\n");
    intervalo = verifica_num(intervalo, 1, 320);

    printf("Bem vindo ao menu do Histórico de Temperaturas! Escolha uma das seguintes opções:\n1 - Global.\n2 - Por país.\n3 - Por cidade.\n4 - Voltar ao menu principal.\n");
    opcao = verifica_num(opcao, 1, 4);

    switch(opcao)
    {
        case 1:
            if(countries_head == NULL) // verifica se o ficheiro dos paises foi carregado
            {
                printf("Esta opção só pode ser realizada se o ficheiro dos países for lido. Operação não realizada.\n");
                break;
            }
            else calcula_temp_global(intervalo, countries_head);
            break;
        case 2:
            if(countries_head == NULL)
            {
                printf("Esta opção só pode ser realizada se o ficheiro dos países for lido. Operação não realizada.\n");
                break;
            }
            else
            {
                introduz_string(regiao);                                                // pede ao utilizador o pais do qual quer ver o historico
                existe = verifica_regiao(countries_head, 1, regiao);                    // verifica se o pais introduzido existe na lista
                if(existe == 1) calcula_temp_pais(intervalo, countries_head, regiao);
                else printf("O país introduzido não existe no ficheiro lido.\n");
            }
            break;

        case 3:
            if(cities_head == NULL) // verifica se o ficheiro das cidades foi carregado
            {
                printf("Esta opção só pode ser realizada se o ficheiro das cidades for lido. Operação não realizada.\n");
                break;
            }
            else
            {
                introduz_string(regiao);                                    // pede ao utilizador a cidade do qual quer ver o historico
                existe = verifica_regiao(cities_head, 2, regiao);
                if(existe == 1) calcula_temp_cidades(intervalo, cities_head, regiao);
                else printf("A cidade introduzida não existe no ficheiro lido.\n");
            }
            break;

        case 4:
            return;
            break;

        default:
            printf("Erro. Número inválido.\n");
    }
}

// função que realiza a opção 1 do historico de temperaturas

void calcula_temp_global(int intervalo, node_t *contas_head)
{
    node_t *aux = contas_head;
    int counter = 0, divisor = 0, linha = 0;
    float maximo = -1000.0f, minimo = 1000.0f, soma = 0.0f;
    char acao[MAX_CHAR] = "";

    while(aux->next != NULL)
    {
        if(aux->payload.temperatura > maximo) maximo = aux->payload.temperatura;
        if(aux->payload.temperatura < minimo) minimo = aux->payload.temperatura;
        soma = soma + aux->payload.temperatura;
        divisor++;

        counter = counter + aux->next->payload.dt.ano - aux->payload.dt.ano;

        if(counter >= intervalo)    // apos serem processados os valores de um numero de anos igual ao intrevalo é impressa uma linha no ecrã
        {
            printf("%d-%d. Máximo: %f Mínimo: %f Média: %f\n", aux->payload.dt.ano-intervalo+1, aux->payload.dt.ano, maximo, minimo, (soma/divisor));
            linha++;
            counter = counter - intervalo;

            //reset das variavies
            maximo = -1000.0f;
            minimo = 1000.0f;
            soma = 0.0f;
            divisor = 0;

        }

        if(linha == 20)  // quando são impressas 20 linhas é perguntado ao utilizador se quer continuar ou a ver valores ou se quer sair
        {
            linha = 0;
            acao[0] = '\n';
            while(acao[0] != 'a' && acao[0] != 'q')
            {
                printf("Prima 'a' para aceder aos próximos valores e 'q' para sair\n");
                fgets(acao, 5, stdin);
            }

            if(acao[0] == 'q') break;
        }

        aux = aux->next;
    }

    //como o ultimo intervalo não pode ser menor que o intervalo introduzido pelo utilizador então o ultimo intervalo é impresso aqui
    if(acao[0] != 'q') printf("%d-%d. Máximo: %f Mínimo: %f Média: %f\n", aux->payload.dt.ano-counter, aux->payload.dt.ano, maximo, minimo, (soma/divisor));
}

//aplica a opção 2 do historico de temperatura
void calcula_temp_pais(int intervalo, node_t *contas_head, char regiao[])
{
    node_t *aux = contas_head;
    int counter = 0, divisor = 0, linha = 0;
    float maximo = -1000.0f, minimo = 1000.0f, soma = 0.0f;
    char acao = '\0';

    while(aux->next != NULL)
    {
        if(strcmp(regiao, aux->payload.pais) == 0)
        {
            if(aux->payload.temperatura > maximo) maximo = aux->payload.temperatura;
            if(aux->payload.temperatura < minimo) minimo = aux->payload.temperatura;
            soma = soma + aux->payload.temperatura;
            divisor++;
        }
            counter = counter + aux->next->payload.dt.ano - aux->payload.dt.ano;

            if(counter >= intervalo)
            {
                if(divisor == 0) continue;
                printf("%d-%d. Máximo: %f Mínimo: %f Média: %f\n", aux->payload.dt.ano-intervalo+1, aux->payload.dt.ano, maximo, minimo, (soma/divisor));
                counter = counter - intervalo;
                maximo = -1000.0f;
                minimo = 1000.0f;
                soma = 0.0f;
                divisor = 0;
                linha++;
            }

            if(linha == 20)
            {
                linha = 0;
                acao = '\0';

                while(acao != 'a' && acao != 'q')
                {
                    acao = '\0';
                    printf("Prima 'a' para aceder aos próximos valores e 'q' para sair\n");
                    scanf(" %c", &acao);
                }

                if(acao == 'q') break;
            }

            aux = aux->next;
    }

    if(acao != 'q') printf("%d-%d. Máximo: %f Mínimo: %f Média: %f\n", aux->payload.dt.ano-counter, aux->payload.dt.ano, maximo, minimo, (soma/divisor));
}

// aplica a opção 3 do historico de temperatura
void calcula_temp_cidades(int intervalo, node_t* head,char regiao[])
{
    int counter = 0, divisor = 0, linha = 0;
    float maximo = -1000.0f, minimo = 1000.0f, soma = 0.0f;
    char acao = '\0';
    node_t* aux = head, *last_citie = NULL;

    while (aux->next != NULL)
    {
        if(strcmp(regiao,aux->payload.cidade)==0)
        {
            if(aux->payload.temperatura > maximo) maximo = aux->payload.temperatura;
            if(aux->payload.temperatura < minimo) minimo = aux->payload.temperatura;
            soma = soma + aux->payload.temperatura;
            divisor++;

            if(strcmp(regiao,aux->next->payload.cidade)==0){
            counter = counter + aux->next->payload.dt.ano - aux->payload.dt.ano;}

            last_citie=aux;
        }


        if(counter >= intervalo)
        {
            printf("%d-%d. Máximo: %f Mínimo: %f Média: %f\n", aux->payload.dt.ano-intervalo+1, aux->payload.dt.ano, maximo, minimo, (soma/divisor));
            counter = counter - intervalo;
            maximo = -1000.0f;
            minimo = 1000.0f;
            soma = 0.0f;
            divisor = 0;
            linha++;
        }

        if(linha == 20)
        {
            linha = 0;
            acao = '\0';

            while(acao != 'a' && acao != 'q')
            {
                acao = '\0';
                printf("Prima 'a' para aceder aos próximos valores e 'q' para sair\n");
                scanf(" %c", &acao);
            }

            if(acao == 'q') break;
        }

        aux=aux->next;

    }

    if(acao != 'q') printf("%d-%d. Máximo: %f Mínimo: %f Média: %f\n", last_citie->payload.dt.ano-counter, last_citie->payload.dt.ano, maximo, minimo, (soma/divisor));
}

//aplica a opcao 3 do modo de texto (analise temperatura por ano)

void analise_ano(node_t* head_countries, node_t* head_cities)
{
    int ano = 0, n = 0, opcao = 0;
    n_analise* head = NULL;

    printf("Insira o ano que deseja analizar.\n");
    ano = verifica_num(ano, 1700, 2020);
    printf("Insira o número de regiões que quer ver.\n");
    n = verifica_num(n, 1, 20);

    printf("Escolha uma das seguintes opcões:\n1 - Análise por pais.\n2 - Análise por cidade.\n");
    opcao = verifica_num(opcao, 1, 3);

    switch(opcao)
    {
        case 1:
            if (head_countries != NULL) // verifica se o ficheiro dos paises fui carregado
            {
                 head = analise_ano_calculos (head_countries, ano, 1); //cria uma lista

                 //imprime os n pais mais quente, mais frios, mais extremos usandos os dados da lista criada anteriormente
                 if(head != NULL) imprime_n(head, n);
                 else printf("Impossível perante os critérios de filtragem introduzidos.\n");
            }
            else
            {
                printf("Não introduziu o ficheiro correspondente.\nVoltando ao menu principal.\n");
            }
        break;

    case 2:
        if (head_cities != NULL)// verifca se o ficheiro das cidades fui carregado
        {
            head = analise_ano_calculos(head_cities, ano, 2); //cria uma lista

            //imprime os n pais mais quente, mais frios, mais extremos usandos os dados da lista criada anteriormente
            if(head != NULL) imprime_n(head, n);
            else printf("Impossível perante os critérios introduzidos.\n");

        }
        else
        {
            printf("Não introduziu o ficheiro correspondente.\nVoltando ao menu principal.\n");
        }
        break;

    case 3:
        return;

    default:
        printf("Número inválido.\n");
    }

    if(head != NULL) free_memory_2(head);

    return;
}

n_analise* analise_ano_calculos(node_t* head, int ano, int modo) // modo=1 analise por pais , modo=2 analise por cidade
{
    n_analise *head_analise = NULL, *new_node = NULL, *aux_2 = NULL;
    node_t *aux = head;
    int procura = 0;

    while(aux != NULL)
    {
        procura = 0;
        if(ano == aux->payload.dt.ano )
        {
            aux_2 = head_analise;

            while(aux_2 != NULL)

            {   //verifica já existe um node com o nome da cidade/pais na lista que está a ser criada
                if((strcmp(aux_2->local, aux->payload.cidade) == 0 && modo==2) || (strcmp(aux_2->local,aux->payload.pais)==0 && modo==1) )
                {
                    aux_2->divisor++;
                    aux_2->soma = aux_2->soma + aux->payload.temperatura;
                    if(aux->payload.temperatura > aux_2->maximo) aux_2->maximo = aux->payload.temperatura;
                    if(aux->payload.temperatura < aux_2->minimo ) aux_2->minimo = aux->payload.temperatura;
                    procura=1;
                    break;
                }
                aux_2 = aux_2->next;
            }
//  se procura=0 então não existe um node com o a cidade/pais em causa e um node novo é criado
            if(procura == 0)
            {
                new_node = checkedMalloc(sizeof(n_analise));

                if(modo == 1) strcpy(new_node->local, aux->payload.pais);
                if(modo == 2) strcpy(new_node->local, aux->payload.cidade);

                new_node->maximo = aux->payload.temperatura;
                new_node->minimo = aux->payload.temperatura;
                new_node->soma = aux->payload.temperatura;
                new_node->divisor = 1;
                new_node->next = head_analise;
                head_analise = new_node;
            }
        }
        aux = aux->next;
    }

    return head_analise;
}

void imprime_n(n_analise* head, int n)
{
    int i = 0, j = 0;
    n_analise *aux = head, *m_quente = NULL, *m_frias = NULL, *m_extremas = NULL;
    float ultimo_max = 1000, maximo = -100, ultimo_min =-100, minimo = 100, ultimo_extremo = 1000, extremo = -100;

    printf("Os %d mais quentes:  Os %d mais frios:    Os %d mais extremos:\n", n, n, n);

    for(i = 0; i < n; i++)
    {
        aux = head;
        while(aux != NULL)
        {
            //encontra  a maior media na lista que é menor que a ultima média impressa
            if(aux->soma / aux->divisor < ultimo_max && aux->soma / aux->divisor > maximo )
            {
                maximo = aux->soma / aux->divisor;
                m_quente = aux;
            }
            //encontra a menor média que é maior que a ultima média impressa
        if(aux->soma/aux->divisor > ultimo_min && aux->soma/aux->divisor < minimo )
            {
              minimo=aux->soma / aux->divisor;
              m_frias = aux;
            }
            //encontra  a maior diferença entre temperatura maxima e minima que é menor que a ultima temperatura impressa
        if (aux->maximo-aux->minimo < ultimo_extremo && aux->maximo- aux->minimo > extremo )
            {
                extremo = aux->maximo - aux->minimo;
                m_extremas = aux;
            }


            aux=aux->next;
        }

        ultimo_max = maximo;
        maximo = -100;
        ultimo_min = minimo;
        minimo = 100;
        ultimo_extremo = extremo;
        extremo = -100;

        printf("%s", m_quente->local);

        for(j = strlen(m_quente->local); j < 20; j++) //cria colunas alinhada
            printf(" ");


        printf("%s", m_frias->local);

        for(j = strlen(m_frias->local); j < 20; j++) //cria colunas alinhadas

            printf(" ");


        printf("%s\n", m_extremas->local);
    }

}

//aplica a opção 4 do modo de texto (analise da temp global)
void analise_global(node_t *countries_head, node_t *cities_head)
{
    int opcao = 0, m = 0;

    printf("Insira o tamanho de intervalo que quer estudar os anos.\n");
    m = verifica_num(m, 1, 12);

    printf("Escolha uma das seguintes opções:\n1 - Alteração da temperatura global do planeta.\n2 - Alteração da temperatura global de um país.\n3 - Aumento da temperatura global de uma cidade.\n");
    opcao = verifica_num(opcao, 1, 4);

    switch (opcao)
    {
        case 1:
            if(countries_head != NULL)
            {
                moving_average_global(countries_head, m);
            }
            else
            {
                printf("Não leu o ficheiro correspondente.\n");
            }
            break;
        case 2:
            if(countries_head != NULL)
            {
                moving_average_pais(countries_head, m);
            }
            else
            {
                printf("Não leu o ficheiro correspondente.\n");
            }
            break;

        case 3:
            if(cities_head != NULL)
            {
                moving_average_cidade(cities_head, m);
            }
            else
            {
                printf("Não leu o ficheiro correspondente.\n");
            }
            break;

        case 4:
            printf("Voltando ao menu principal.\n");
            return;

        default:
            printf("Número inválido.\n");
    }
}

//aplica opção 1 do menu da analise da temperatura global
void moving_average_global(node_t* head, int m)
{
    node_t *aux = NULL;
    int i = 0, j = 0, d = 0, divisor = 0;
    float media[12][320] = {{0.0f}}, media_ma[12][320] = {{0.0f}}, ma_vector[320] = {0}, soma = 0.0f;

    aux = head;
// criar a matriz com a media da temperatura
    i = head->payload.dt.mes;

    while(aux->next != NULL)
    {
        if(aux->payload.dt.ano == aux->next->payload.dt.ano)
        {
            if(aux->payload.dt.mes == aux->next->payload.dt.mes) // se o mes e o ano atual forem iguais ao mes e ano do aux->next a soma e o divisor são atulizadas
            {
                soma = soma + aux->payload.temperatura;
                divisor++;
            }
            else// se o mes mudar o os valores da soma e divisores são atualizado,os dados são colocados na respectiva posição da matriz
            {
                soma = soma + aux->payload.temperatura;
                divisor++;
                media[i][j] = soma / divisor;
                soma = 0.0f;
                divisor = 0;
                i = aux->next->payload.dt.mes - 1;// i toma o valor do proximo mes

            }
        }
        else // se o mes mudar o os valores da soma e divisores são atualizado,os dados são colocados na respectiva posição da matriz
        {
            soma = soma + aux->payload.temperatura;
            divisor++;
            media[i][j] = soma / divisor;
            soma = 0.0f;
            divisor = 0;
            j = j + aux->next->payload.dt.ano - aux->payload.dt.ano; // j é atualizado
            i = aux->next->payload.dt.mes - 1;// i toma o valor do proximo mes
        }
        aux = aux->next;
    }

    soma = 0.0f;
    divisor = 0;


// criar a matriz com a moving average das temperaturas
    for(i = 0; i < 12; i++)
    {
        for(j = 0; j < 113 - m + 1; j++)
        {
            for(d = 0; d < m; d++)
            {
                soma = soma + media[i][d + j];
                if(media[i][d + j] != 0) divisor++;
            }
            if(divisor != 0) media_ma[i][j + m - 1] = soma / divisor;
            soma = 0.0f;
            divisor = 0;
        }
    }

// cria um vector com as medias das temperaturas em cada ano
    for(j = 0; j < 113; j++)
    {
        for(i = 0; i < 12; i++)
        {
            soma = soma + media_ma[i][j];
            if(media_ma[i][j] != 0.0f) divisor++;
        }
        if(divisor != 0) ma_vector[j] = soma / divisor;
        soma = 0.0f;
        divisor = 0;
    }

    imprime_ma(ma_vector, 1860, head->payload.dt.ano);
    imprime_ma(ma_vector, 1910, head->payload.dt.ano);
    imprime_ma(ma_vector, 1960, head->payload.dt.ano);
    imprime_ma(ma_vector, 1990, head->payload.dt.ano);
    imprime_ma(ma_vector, 2013, head->payload.dt.ano);
}

//aplica opção 2 do menu da analise da temperatura global, semelhante à função anterior
//mas com uma condição para só considerar o pais desejado
void moving_average_pais(node_t* head, int m)
{
    node_t *aux = NULL;
    int i = 0, j = 0, d = 0, divisor = 0;
    float media[12][320] = {{0.0f}}, media_ma[12][320] = {{0.0f}}, ma_vector[320] = {0}, soma = 0.0f;
    char regiao[MAX_CHAR] = "";

    introduz_string(regiao);
    if(verifica_regiao(head, 1, regiao) == 1)
    {

        aux = head;

        i = head->payload.dt.mes;

        while(aux->next != NULL)
        {
            if(aux->payload.dt.ano == aux->next->payload.dt.ano)
            {
                if(aux->payload.dt.mes == aux->next->payload.dt.mes)
                {
                    if(strcmp(regiao, aux->payload.pais) == 0)
                    {
                        soma = soma + aux->payload.temperatura;
                        divisor++;
                    }
                }
                else
                {
                    if(strcmp(regiao, aux->payload.pais) == 0)
                    {
                        soma = soma + aux->payload.temperatura;
                        divisor++;
                    }
                    media[i][j] = soma / divisor;
                    soma = 0.0f;
                    divisor = 0;
                    i = aux->next->payload.dt.mes - 1;
                }
            }
            else
            {
                if(strcmp(regiao, aux->payload.pais) == 0)
                {
                    soma = soma + aux->payload.temperatura;
                    divisor++;
                }
                media[i][j] = soma / divisor;
                soma = 0.0f;
                divisor = 0;
                j = j + aux->next->payload.dt.ano - aux->payload.dt.ano;
                i = aux->next->payload.dt.mes - 1;
            }

            aux = aux->next;
        }

        soma = 0.0f;
        divisor = 0;

        for(i = 0; i < 12; i++)
        {
            for(j = 0; j < 113 - m + 1; j++)
            {
                for(d = 0; d < m; d++)
                {
                    soma = soma + media[i][d + j];
                    if(media[i][d + j] != 0) divisor++;
                }
                if(divisor != 0) media_ma[i][j + m - 1] = soma / divisor;
                soma = 0.0f;
                divisor = 0;
            }
        }

        for(j = 0; j < 113; j++)
        {
            for(i = 0; i < 12; i++)
            {
                soma = soma + media_ma[i][j];
                if(media_ma[i][j] != 0.0f) divisor++;
            }
            if(divisor != 0) ma_vector[j] = soma / divisor;
            soma = 0.0f;
            divisor = 0;
            printf("%f",ma_vector[j]);
        }

        imprime_ma(ma_vector, 1860, head->payload.dt.ano);
        imprime_ma(ma_vector, 1910, head->payload.dt.ano);
        imprime_ma(ma_vector, 1960, head->payload.dt.ano);
        imprime_ma(ma_vector, 1990, head->payload.dt.ano);
        imprime_ma(ma_vector, 2013, head->payload.dt.ano);
    }
    else printf("Este país não existe no ficheiro.\n");
}

//aplica opção 2 do menu da analise da temperatura global, semelhante à função anterior
//mas com uma condição para só considerar a cidade desejada
void moving_average_cidade(node_t *head, int m)
{
    node_t *aux = NULL;
    int i = 0, j = 0, d = 0, divisor = 0,ano_i=0,encontrado=0;
    float media[12][320] = {{0.0f}}, media_ma[12][320] = {{0.0f}}, ma_vector[320] = {0}, soma = 0.0f;
    char regiao[MAX_CHAR] = "";

    introduz_string(regiao);
    if(verifica_regiao(head, 2, regiao) == 1)
    {

        aux = head;

        i = head->payload.dt.mes;

        while(aux->next != NULL)
        {
            if(strcmp(regiao, aux->payload.cidade) == 0)
            {
                if (encontrado==0)ano_i=aux->payload.dt.ano;
                encontrado=1;
                if(aux->payload.dt.ano == aux->next->payload.dt.ano)
                {
                    if(aux->payload.dt.mes == aux->next->payload.dt.mes)
                    {
                        soma = soma + aux->payload.temperatura;
                        divisor++;
                    }
                    else
                    {
                        soma = soma + aux->payload.temperatura;
                        divisor++;
                        media[i][j] = soma / divisor;
                        soma = 0.0f;
                        divisor = 0;
                        i = aux->next->payload.dt.mes - 1;

                    }
                }
                else
                {
                    soma = soma + aux->payload.temperatura;
                    divisor++;
                    media[i][j] = soma / divisor;
                    soma = 0.0f;
                    divisor = 0;
                    j = j + aux->next->payload.dt.ano - aux->payload.dt.ano;
                    i = aux->next->payload.dt.mes - 1;
                }
            }
            aux = aux->next;
        }

        soma = 0.0f;
        divisor = 0;

        for(i = 0; i < 12; i++)
        {
            for(j = 0; j < 113 - m + 1; j++)
            {
                for(d = 0; d < m; d++)
                {
                    soma = soma + media[i][d + j];
                    if(media[i][d + j] != 0) divisor++;
                }
                if(divisor != 0) media_ma[i][j + m - 1] = soma / divisor;
                soma = 0.0f;
                divisor = 0;
            }
        }

        for(j = 0; j < 113; j++)
        {
            for(i = 0; i < 12; i++)
            {
                soma = soma + media_ma[i][j];
                if(media_ma[i][j] != 0.0f) divisor++;
            }
            if(divisor != 0) ma_vector[j] = soma / divisor;
            soma = 0.0f;
            divisor = 0;
        }

        imprime_ma(ma_vector, 1860, ano_i);
        imprime_ma(ma_vector, 1910, ano_i);
        imprime_ma(ma_vector, 1960, ano_i);
        imprime_ma(ma_vector, 1990, ano_i);
        imprime_ma(ma_vector, 2013, ano_i);
    }
}

//calcula os valores máximos e mínimos e com isso calcula o aquecimento.
void imprime_ma(float ma_vector[], int ano, int ano_i)
{
    int j=0;
    float maximo = -100.0f, minimo = 100.0f;

    for(j = 0; j < ano - ano_i; j++)
    {
        if(ma_vector[j] != 0.0f)
        {
            if(ma_vector[j] > maximo) maximo = ma_vector[j];
            if(ma_vector[j] < minimo) minimo = ma_vector[j];
        }
    }

    if (maximo != -100) printf("O aumento da temperatura entre %d e %d foi de %f\n", ano_i,ano,maximo-minimo);
}
