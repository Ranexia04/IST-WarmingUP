#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <string.h>
#include "file.h"

void main_grafico(node_t *f2_head, int ano_i, int ano_f, float temp_min, float temp_max)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *imgs[2];
    SDL_Event event;
    int width = TABLE_SIZE + LEFT_BAR_SIZE;
    int height = TABLE_HEIGHT;
    int quit = 0, cor = 0;
    int offset_x = 0, offset_y = 0;
    node_t *aux = f2_head;
    int stop = 0, i = 0, divisor = 0;
    float soma = 0.0f, intervalo = 0.0f;
    int height_dados = 0;

    intervalo = temp_max - temp_min;
    intervalo = intervalo / 15;//calcula o intervalo para distiguir a cor que vai renderizar

    InitEverything(width, height, &serif, imgs, &window, &renderer);

    height_dados = 0;
    height_dados = RenderTable(serif, imgs, renderer, height_dados);//faz render table e
    //retorna o valor da posição y na qual vai por o nome da cidade e a sua temperatura na barra branca

    while(quit == 0)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit = 1;// se quit == 1, sai do programa
            }
            else if(event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_q:
                    quit = 1;// se quit == 1, sai do programa
                    break;
                }
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN && stop == 1)// se stop == 1, significa que os pontos foram renderizados no ecrã
            // ao longo do tempo e partir deste momento é possivel clicar no ecrã
            {
                aux = f2_head;
                cor = 0;

                while(aux->next != NULL)
                {
                    if(strcmp(aux->payload.cidade, aux->next->payload.cidade) != 0)// este if serve para que o valor da temperatura a renderizar
                    //seja o mais recente
                    {
                        // dependendo localização do mapa o sinal dos offsets varia.
                        if(aux->payload.latitude.direcao == 'S' && aux->payload.longitude.direcao == 'E')
                        {
                            //apenas aparece a temperatura se clicar no sitio correto
                            if(event.button.y > MIDDLE_H + (int)(aux->payload.latitude.angular*3.13333f) - 8
                            && event.button.y < MIDDLE_H + (int)(aux->payload.latitude.angular*3.13333f) + 8
                            && event.button.x > MIDDLE_W + (int)(aux->payload.longitude.angular*3.13333f) - 8
                            && event.button.x < MIDDLE_W + (int)(aux->payload.longitude.angular*3.13333f) + 8)
                            {
                                show_data(height_dados, aux, serif, renderer);
                                SDL_RenderPresent(renderer);
                                break;
                            }
                        }
                        if(aux->payload.latitude.direcao == 'S' && aux->payload.longitude.direcao == 'W')
                        {
                            if(event.button.y > MIDDLE_H + (int)(aux->payload.latitude.angular*3.13333f) - 8
                            && event.button.y < MIDDLE_H + (int)(aux->payload.latitude.angular*3.13333f) + 8
                            && event.button.x > MIDDLE_W - (int)(aux->payload.longitude.angular*3.13333f) - 8
                            && event.button.x < MIDDLE_W - (int)(aux->payload.longitude.angular*3.13333f) + 8)
                            {
                                show_data(height_dados, aux, serif, renderer);
                                SDL_RenderPresent(renderer);
                                break;
                            }
                        }
                        if(aux->payload.latitude.direcao == 'N' && aux->payload.longitude.direcao == 'E')
                        {
                            if(event.button.y > MIDDLE_H - (int)(aux->payload.latitude.angular*3.13333f) - 8
                            && event.button.y < MIDDLE_H - (int)(aux->payload.latitude.angular*3.13333f) + 8
                            && event.button.x > MIDDLE_W + (int)(aux->payload.longitude.angular*3.13333f) - 8
                            && event.button.x < MIDDLE_W + (int)(aux->payload.longitude.angular*3.13333f) + 8)
                            {
                                show_data(height_dados, aux, serif, renderer);
                                SDL_RenderPresent(renderer);
                                break;
                            }
                        }
                        if(aux->payload.latitude.direcao == 'N' && aux->payload.longitude.direcao == 'W')
                        {
                            if(event.button.y > MIDDLE_H - (int)(aux->payload.latitude.angular*3.13333f) - 8
                            && event.button.y < MIDDLE_H - (int)(aux->payload.latitude.angular*3.13333f) + 8
                            && event.button.x > MIDDLE_W - (int)(aux->payload.longitude.angular*3.13333f) - 8
                            && event.button.x < MIDDLE_W - (int)(aux->payload.longitude.angular*3.13333f) + 8)
                            {
                                show_data(height_dados, aux, serif, renderer);
                                SDL_RenderPresent(renderer);
                                break;
                            }
                        }
                    }
                    aux = aux->next;
                }
                aux = f2_head;
            }
            else if(stop == 0)//impede de correr mais vezes
            {
                //coloca um circulo a preto em todas as localizações do ecrã
                while(aux->next != NULL)
                {
                    if(strcmp(aux->payload.cidade, aux->next->payload.cidade) != 0)
                    {
                        if(aux->payload.latitude.angular != 0.0f && aux->payload.longitude.angular != 0.0f)
                        {
                            convertCoords(aux->payload.latitude.angular, aux->payload.latitude.direcao,
                            aux->payload.longitude.angular, aux->payload.longitude.direcao, &offset_x, &offset_y);
                            filledCircleRGBA(renderer, cor, offset_x, offset_y);
                        }
                    }
                    aux = aux->next;
                }
                SDL_RenderPresent(renderer);
                SDL_Delay(30);
                aux = f2_head;

                for(i = ano_i; i < ano_f; i = i + 2)// percorre a lista para cada vez que passam 2 anos até chegar ao ano final
                {
                    while(aux->next != NULL)
                    {
                        if(aux->payload.dt.ano < i + 2 && aux->payload.dt.ano >= i)//apenas conta para a média se o ano estiver correto
                        {
                            soma = soma + aux->payload.temperatura;
                            divisor++;
                        }

                        if(divisor == 0)//prevenção de erro no caso de não existirem dados
                        {
                            aux = aux->next;
                            continue;
                        }

                        if(strcmp(aux->payload.cidade, aux->next->payload.cidade) != 0)
                        {
                            if(soma/divisor < temp_min + intervalo) cor = 1;// seleciona a cor ao comparar a cor com todos os intervalos
                            else if(soma/divisor >= temp_min + intervalo && soma/divisor < temp_min + 2*intervalo) cor = 2;
                            else if(soma/divisor >= temp_min + 2*intervalo && soma/divisor < temp_min + 3*intervalo) cor = 3;
                            else if(soma/divisor >= temp_min + 3*intervalo && soma/divisor < temp_min + 4*intervalo) cor = 4;
                            else if(soma/divisor >= temp_min + 4*intervalo && soma/divisor < temp_min + 5*intervalo) cor = 5;
                            else if(soma/divisor >= temp_min + 5*intervalo && soma/divisor < temp_min + 6*intervalo) cor = 6;
                            else if(soma/divisor >= temp_min + 6*intervalo && soma/divisor < temp_min + 7*intervalo) cor = 7;
                            else if(soma/divisor >= temp_min + 7*intervalo && soma/divisor < temp_min + 8*intervalo) cor = 8;
                            else if(soma/divisor >= temp_min + 8*intervalo && soma/divisor < temp_min + 9*intervalo) cor = 9;
                            else if(soma/divisor >= temp_min + 9*intervalo && soma/divisor < temp_min + 10*intervalo) cor = 10;
                            else if(soma/divisor >= temp_min + 10*intervalo && soma/divisor < temp_min + 11*intervalo) cor = 11;
                            else if(soma/divisor >= temp_min + 11*intervalo && soma/divisor < temp_min + 12*intervalo) cor = 12;
                            else if(soma/divisor >= temp_min + 12*intervalo && soma/divisor < temp_min + 13*intervalo) cor = 13;
                            else if(soma/divisor >= temp_min + 13*intervalo && soma/divisor < temp_min + 14*intervalo) cor = 14;
                            else cor = 15;

                            convertCoords(aux->payload.latitude.angular, aux->payload.latitude.direcao, aux->payload.longitude.angular, aux->payload.longitude.direcao, &offset_x, &offset_y);
                            filledCircleRGBA(renderer, cor, offset_x, offset_y);
                            soma = 0;
                            divisor = 0;
                        }
                        aux = aux->next;
                    }
                    aux = f2_head;
                    SDL_RenderPresent(renderer);
                    SDL_Delay(30);
                }
                printf("Dados exibidos.\n");
                stop = 1;//impede de correr mais vezes
            }
        }
    }

    SDL_RenderClear(renderer);//quit
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Saíndo do programa.\n");
}

int RenderTable(TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer , int height)
{
    SDL_Color black = { 0, 0, 0 };
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = TABLE_SIZE;
    tableDest.h = TABLE_HEIGHT;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(TABLE_SIZE, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(TABLE_SIZE+3*MARGIN, height, "Henrique Lourenco", _font, &black, _renderer);

    // this renders the student number
    RenderText(TABLE_SIZE+3*MARGIN, height, "IST190092", _font, &black, _renderer);

    height += RenderText(TABLE_SIZE+3*MARGIN, height, " ", _font, &black, _renderer);

    height += RenderText(TABLE_SIZE+3*MARGIN, height, "Rodrigo Aires", _font, &black, _renderer);

    RenderText(TABLE_SIZE+3*MARGIN, height, "IST190182", _font, &black, _renderer);

    height += RenderText(TABLE_SIZE+3*MARGIN, height, " ", _font, &black, _renderer);//serve para "incrementar" o height

    // destroy everything
    SDL_DestroyTexture(table_texture);

    return height;//retorna o height para ser utilizado na impressao das temperaturas na barra branca
}

void convertCoords(float lat_ang, char lat_dir, float long_ang, char long_dir, int *_offset_x, int *_offset_y)
{
    if(lat_dir == 'S') *_offset_y = (int)(3.13333f*lat_ang);// converte os angulos para pixeis
    else *_offset_y = -(int)(3.13333f*lat_ang);

    if(long_dir == 'E') *_offset_x = (int)(3.13333f*long_ang);
    else *_offset_x = -(int)(3.13333f*long_ang);
}

void filledCircleRGBA(SDL_Renderer * _renderer, int cor, int offset_x, int offset_y)
{
    int off_x = 0;
    int off_y = 0;
    float degree = 0.0;
    int circleX = MIDDLE_W + offset_x;
    int circleY = MIDDLE_H + offset_y;
    int circleR = 8;
    float step = M_PI / (circleR*8);

    if(cor == 0) SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    if(cor == 1) SDL_SetRenderDrawColor(_renderer, 0, 0, 204, 255);
    if(cor == 2) SDL_SetRenderDrawColor(_renderer, 51, 51, 255, 255);
    if(cor == 3) SDL_SetRenderDrawColor(_renderer, 102, 102, 255, 255);
    if(cor == 4) SDL_SetRenderDrawColor(_renderer, 102, 178, 255, 255);
    if(cor == 5) SDL_SetRenderDrawColor(_renderer, 51, 153, 255, 255);
    if(cor == 6) SDL_SetRenderDrawColor(_renderer, 153, 204, 255, 255);
    if(cor == 7) SDL_SetRenderDrawColor(_renderer, 102, 255, 255, 255);
    if(cor == 8) SDL_SetRenderDrawColor(_renderer, 0, 204, 204, 255);
    if(cor == 9) SDL_SetRenderDrawColor(_renderer, 255, 102, 178, 255);
    if(cor == 10) SDL_SetRenderDrawColor(_renderer, 255, 0, 127, 255);
    if(cor == 11) SDL_SetRenderDrawColor(_renderer, 255, 153, 51, 255);
    if(cor == 12) SDL_SetRenderDrawColor(_renderer, 204, 102, 0, 255);
    if(cor == 13) SDL_SetRenderDrawColor(_renderer, 255, 51, 51, 255);
    if(cor == 14) SDL_SetRenderDrawColor(_renderer, 204, 0, 0, 255);
    if(cor == 15) SDL_SetRenderDrawColor(_renderer, 102, 0, 0, 255);

    while (circleR > 0)// renderiza o circulo
    {
        for (degree = 0.0; degree < M_PI/2; degree+=step)
        {
            off_x = (int)(circleR * cos(degree));
            off_y = (int)(circleR * sin(degree));
            SDL_RenderDrawPoint(_renderer, circleX+off_x, circleY+off_y);
            SDL_RenderDrawPoint(_renderer, circleX-off_y, circleY+off_x);
            SDL_RenderDrawPoint(_renderer, circleX-off_x, circleY-off_y);
            SDL_RenderDrawPoint(_renderer, circleX+off_y, circleY-off_x);
        }
        circleR--;
    }
}

void show_data(int height_dados, node_t * aux, TTF_Font *_font, SDL_Renderer* _renderer)
{
    char temp[50];
    SDL_Color black = { 0, 0, 0 };

    snprintf(temp, sizeof(temp), "%.4f", aux->payload.temperatura);

    SDL_Rect rectangulo = {(TABLE_SIZE + 3*MARGIN), height_dados, 140, 30};//estes retangulos branco servem apenas para
    //dar overwrite do valor clicado anterior
    SDL_RenderFillRect(_renderer, &rectangulo);

    height_dados += RenderText(TABLE_SIZE+3*MARGIN, height_dados, aux->payload.cidade, _font, &black, _renderer);

    SDL_Rect rectangulo_2 = {(TABLE_SIZE + 3*MARGIN), height_dados, 140, 30};
    SDL_RenderFillRect(_renderer, &rectangulo_2);

    RenderText(TABLE_SIZE+3*MARGIN, height_dados, temp, _font, &black, _renderer);
}

int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("mapa_mundo.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "WarmingUp", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}
