#include "global.h"

enum enumeracao1 funcionalidade_menu()
{
    BITMAP *buffer;
    BITMAP *menu[4];
    int i, m=0;

    //CARREGANDO DE BITMAPS
    buffer = create_bitmap(2*X_MAX, Y_MAX);
    menu[0] = load_bitmap("menu/0.bmp", NULL);
    menu[1] = load_bitmap("menu/1.bmp", NULL);
    menu[2] = load_bitmap("menu/2.bmp", NULL);
    menu[3] = load_bitmap("menu/3.bmp", NULL);

    rest(100);
    while (!key[KEY_ENTER]){     // FECHA ESTE MENU, DEPOIS CONFIGURAR PARA VOLTAR UMA VEZ

          masked_blit(menu[m], screen, 0, 0,0,0,X_MAX,Y_MAX);
          rest(80);
          if ((key[KEY_DOWN]) && (m != 3)){
             m++;
          }
          if ((key[KEY_UP]) && (m != 0)){
             m--;
          }

    }

    /*Destruindo o que ja utilizamos*/
    destroy_bitmap(buffer);
    for(i=0;i<4;i++)
        destroy_bitmap(menu[i]);
    return m;
  }

int *funcionalidade_carregarposicoes( int *nposicoes)
{
    int tamanho, i, *posicoes;
    FILE *pontos;
	pontos = fopen("dados/frontal.txt","r");

	if(pontos)
    {
      fscanf(pontos,"%d\n", &tamanho);
      posicoes = malloc(tamanho*sizeof(int));

      for (i=0; i < tamanho; i++)
      {
          fscanf(pontos, "%d\n", &posicoes[i]);
      }
    }
    *nposicoes = tamanho;
    fclose(pontos);

    return posicoes;
}


void Ranking(int *score){

	/*Variables*/
    FONT *fontscore;
    struct_rank *rank;
    FILE *fp;
    BITMAP *rankingbackground, *buffer;
    int i, string_y = 130;
    char string_rank[20];
    char nickname[20];

	/*Loading Variables*/
    buffer = create_bitmap(640,480);
	rankingbackground = load_bitmap("menu/ranking.bmp", 0);
	fontscore = load_font ("fontes/comixloud.pcx", NULL, NULL);
	rank = (struct_rank*)malloc(6*sizeof(struct_rank));
    int verif = 1;

    if ((fp = fopen("dados/ranking.bin","r+b")) == NULL){
            system("PAUSE");
            exit(1);
    }

    /*Reading File*/
    for (i=0;i<6;i++) fread(&rank[i], sizeof(struct_rank), 1, fp);

    //VERIFICA SE PRECISA SALVAR NOVA PONTUACAO
    if (*score > rank[5].score){
        strcpy(rank[5].name, nickname);
        rank[5].score = *score;
    }
    else{
         verif = 0;
    }

    if(verif){
        Ordena(rank, 6);
        /*COPIA O VETOR DE STRUCT DE VOLTA PRO ARQUIVO.*/
        rewind(fp);
        for (i=0;i<6;i++){
              fwrite(&rank[i], sizeof(struct_rank), 1, fp);
        }
    }

	while (!key[KEY_ENTER]) {

        clear_bitmap(buffer);
        draw_sprite(buffer, rankingbackground, 0, 0);

            for(i=0;i<5;i++){
                sprintf(string_rank, "%d) %d", i+1, rank[i].score);
                textout(buffer, fontscore, string_rank, RANK_X, string_y, makecol(255,255,255));
                string_y += 55;
            }
            string_y = 130;


        draw_sprite(screen, buffer, 0, 0);
        rest(100);
    }
free(rank);
fclose(fp);
destroy_bitmap(buffer);
destroy_bitmap(rankingbackground);
destroy_font(fontscore);
}


void Troca(struct_rank *rank, int i, int j){
     int aux;

     aux = rank[i].score;
     rank[i].score = rank[j].score;
     rank[j].score = aux;
}

void Ordena(struct_rank rank[], int n){
   int  i, j;

   for (i=0;i<n;i++){
       for(j=0;j<n-1;j++){
            if (rank[j].score < rank[j+1].score)
                   Troca(rank, j, j+1);
       }
   }
}

enum enumeracao2 Colide(int *emptyflag, int *g, Character *jhonny,int *emptyfields, int fields_num, int *velY, int *walk_index, int *jump_index, int *pulex, int *bgx, int *jumping)
{
                enum enumeracao2 colision_state = VERTICAL;
                /*Colisões*/

                //Frontal
                int index = 0;

                for(index=0;index < fields_num; index+=2)
                {
                    if ((-2*(*bgx)+(*jhonny).x > emptyfields[index]) && ((-2*(*bgx))+(*jhonny).x < emptyfields[index+1]) && ((-2*(*bgx))+(*jhonny).x + JX > emptyfields[index+1])&&((*jhonny).y > 480-PY-JY))
                    {
                            (*jhonny).x -= ((-2*(*bgx))+(*jhonny).x+JX)-emptyfields[index + 1]+1;
                            break;
                    }
                }
                /*Encontra se jhonny está posicionado em um vazio ou não*/
                for(index=0;index < fields_num; index+=2)
                {

                    if (((-2*(*bgx))+(*jhonny).x > emptyfields[index]) && ((-2*(*bgx))+(*jhonny).x + JX <= emptyfields[index+1]))
                    {
                        *emptyflag = 1;
                        *g=1;
                        break;
                    }
                }

                //Vertical
                if(((*jhonny).y == Y_MAX - PY - JY)&&(*pulex)&&!(*emptyflag))
                {
                     colision_state = VERTICAL;
                }
                else if((*jhonny).y > SY)
                {
                    colision_state = LOSS;
                }
                else if((*g))
                {
                    colision_state = NULO;
                }

               return  colision_state;
}

