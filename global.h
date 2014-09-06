#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>

/*Definicoes de impressao*/
#define RANK_X 320
#define PAUSE_X 120
#define PAUSE_Y 190
#define HEART_X 50
#define HEART_Y 10

/*Definicoes do tamanho de tela*/
#define X_MAX 640
#define Y_MAX 480

/*Definicoes do tamanho da plataforma*/
#define PX  2560
#define PY  80

/*Definicoes do tamanho do personagem*/
#define JX 44
#define JY 55

#define JX_I 20

/*Definicoes dos espinhos*/
#define SY 480-22-JY

/*Definicoes do tamnho do fundo*/
#define FX 640
#define FY 480

/*Definicoes dos limites de acoes*/
#define GRAVITY 3
#define QTD_SPRITE_WALK 6
#define QTD_SPRITE_JUMP 19
#define ACCEL 0.004



typedef struct
{
    int lives;
    int score;
    int x;
    int y;
}Character;

typedef struct{
    char name[20];
    int score;
}struct_rank;

/*Declaração das funções e itens de configuracao do allegro*/
void init();
void deinit();
enum enumeracao1 funcionalidade_menu();
int *funcionalidade_carregarposicoes( int *nposicoes);
void Ordena(struct_rank rank[], int n);
void Troca(struct_rank *rank, int i, int j);
void Ranking(int *score);
enum enumeracao2 Colide(int *emptyflag, int *g, Character *jhonny,int *emptyfields, int fields_num, int *velY, int *walk_index, int *jump_index, int *pulex, int *bgx, int *jumping);

enum enumeracao1 {PLAY,INSTRUCTION,RANKING,EXIT};
enum enumeracao2 {NULO, VERTICAL, HORIZONTAL, LOSS};





