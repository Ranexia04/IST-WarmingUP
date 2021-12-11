#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#define MAX_CHAR 100

#define TABLE_SIZE 1300
#define TABLE_HEIGHT 672
#define LEFT_BAR_SIZE 150
#define WINDOW_POSX 200
#define WINDOW_POSY 200
#define MARGIN 5
#define MIDDLE_W 652
#define MIDDLE_H 335
#define M_PI 3.14159265

typedef struct
{
    int dia;
    int mes;
    int ano;
}data;

typedef struct
{
    float angular;
    char direcao;
}geo_coord;

typedef struct
{
    data dt;
    float temperatura;
    float incerteza;
    char pais[MAX_CHAR];
    char cidade[MAX_CHAR];
    geo_coord latitude;
    geo_coord longitude;
}dados_temp;

typedef struct node
{
    dados_temp payload;
    struct node *next;
    struct node *prev;
}node_t;

typedef struct n_node
{
   char local[MAX_CHAR];
   float soma;
   float maximo;
   float minimo;
   int divisor;
   struct n_node *next;
}n_analise;

void reconhecimento_parametros(int, char*[], int *, int*, int*, int*);
node_t *leitura_dados(int, char*[], int);
void menu_texto(node_t*,node_t*);
int verifica_num(int, int, int);
void filtragem_dados(node_t*, node_t**,node_t*,node_t**);
void pede_criterios(int*,int*);
void pede_mes(int*, int*);
node_t *inter_analise_pais(node_t*,int,int);
node_t *inter_analise_cidade(node_t*,int,int);
node_t *mes_analise(node_t*,int,int);
void historico_temp(node_t*, node_t*);
void analise_ano(node_t*, node_t*);
void analise_global(node_t*, node_t*);
void *checkedMalloc(size_t);
node_t *copia_lista(node_t*, node_t*);
node_t *selecao (int, int, int, int, node_t*);
node_t *apaga_lista(node_t*);
void calcula_temp_global(int, node_t*);
void calcula_temp_pais(int, node_t*, char[]);
void calcula_temp_cidades(int , node_t*, char[]);
void introduz_string(char []);
void free_memory(node_t*);
void free_memory_2(n_analise*);
int verifica_regiao(node_t*, int, char[]);
n_analise* analise_ano_calculos(node_t*, int, int);
void imprime_n(n_analise*, int);
void moving_average_global(node_t*, int);
void moving_average_pais(node_t*, int);
void moving_average_cidade(node_t*, int);
void imprime_ma(float[], int, int);

// modo_grafico
void main_grafico(node_t*, int, int, float, float);
void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderTable(TTF_Font *, SDL_Surface **, SDL_Renderer *, int);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
void convertCoords(float, char, float, char, int*, int*);
void filledCircleRGBA(SDL_Renderer *, int, int, int);
void show_data(int, node_t*, TTF_Font*, SDL_Renderer*);
#endif // FILE_H_INCLUDED
