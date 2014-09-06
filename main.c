#include "global.h"

int main()
{

    init();

    /*Declaração das enumeracoes*/
    enum enumeracao1 menu_state;
    enum enumeracao2 colision_state = VERTICAL;

    /*Buffer que vai ser usado para imprimir as partes*/
    BITMAP *buffer;

     /*Criacao do personagem*/

            Character jhonny;
            jhonny.score = 0;

    /*Loop que chama o menu enquanto a opcao selecionada nao for "SAIR"*/
  do{
    menu_state = funcionalidade_menu();


    switch(menu_state)
     {
        case (PLAY):

            /*Criacao do buffer*/
            buffer = create_bitmap(2*X_MAX, Y_MAX);

            /*Variaveis de som*/
            SAMPLE *jump_sound;
            SAMPLE *loss_sound;
            jump_sound =  load_sample("sounds/Jump_sound.wav");
            loss_sound = load_sample("sounds/Loss_sound.wav");

            /*Variados BITMAPS*/
            BITMAP *pause;
            BITMAP *heart;
            heart = load_bitmap("img/heart.bmp", 0);

            /*Variaveis de fonte*/
            FONT *fontscore;
            fontscore = load_font ("fontes/comixloud.pcx", NULL, NULL);

            /*Variaveis de uso geral*/
            int index;
            int i;
            char str_score[20];

            /*Variaveis do background*/
            BITMAP *background;
            int bgx = 0;
            background = load_bitmap("background/0.bmp",0);


            /*Carrega as coordenadas dos vazios*/
            int *emptyfields, emptyflag = 0;
            int fields_num;
            emptyfields = funcionalidade_carregarposicoes(&fields_num);

            /*Variaveis do Personagem*/
            int walk_index, jump_index;
            jhonny.x = JX_I;       //Posicao do Jhonny em X recebe CONSTATE JX_I
            jhonny.y = Y_MAX - (JY + PY); //Pos. do Jhonny em Y recebe TAM MAX Y DO JOGO ...
            jhonny.lives = 3;
            //Carrega o movimento de andar
            char sprite_str[20];
            BITMAP *walk[6];
            for(walk_index=0; walk_index < 6; walk_index++)
            {
                sprintf(sprite_str,"walk/%d.bmp", walk_index);
                walk[walk_index] = load_bitmap(sprite_str, 0);
            }
            //Carrega o movimento de pular
            BITMAP *jump[19];
            for(jump_index=0; jump_index < 19; jump_index++)
            {
                sprintf(sprite_str,"jump/%d.bmp", jump_index);
                jump[jump_index] = load_bitmap(sprite_str, 0);
            }


            /*Variaveis da plataforma*/
            BITMAP *plataforma;
            plataforma = load_bitmap("platform/0.bmp", 0);

            /*Variaveis de Movimento*/
            int velY = 0;
            float velX = 0;
            int g = 0;
            int jumping = 0;
            walk_index = 0;
            jump_index = 0;
            int pulex = 0;

            /*LOOP PRINCIPAL, JOGO*/
            while(!key[KEY_ESC] && jhonny.lives > 0)
            {
                jhonny.score += 1 + (int)velX;

                clear_bitmap(buffer);
                rest(40);
                draw_sprite(buffer, background, bgx, 0);
                draw_sprite(buffer, background, X_MAX + bgx, 0);
                draw_sprite(buffer, background, X_MAX + 640 + bgx, 0);
                draw_sprite(buffer, plataforma, bgx*2 , Y_MAX - PY);
                draw_sprite(buffer, plataforma, PX + bgx*2 , Y_MAX - PY);

                ( jumping == 1 ) ? (draw_sprite(buffer, jump[jump_index], jhonny.x, jhonny.y), jump_index++, g = 1) : (draw_sprite(buffer, walk[walk_index], jhonny.x, jhonny.y), walk_index++, g = 0);

                if(emptyflag)
                {
                    g = 1;
                }


                if(jump_index >= QTD_SPRITE_JUMP)
                    jump_index = 0;
                else if (walk_index >= QTD_SPRITE_WALK)
                    walk_index = 0;

                /*Impimindo os valores de pontuação e vidas*/
                for(i=0;i<jhonny.lives;i++){
                    draw_sprite(buffer, heart, HEART_X*i, 10);
                }

                sprintf(str_score, "SCORE: %d", jhonny.score);
                textprintf_ex(buffer, fontscore, 10, 50, makecol(255,255,255), -1, "SCORE: %d", jhonny.score);



                //IMPRIME O BUFFER CARREGADO NA TELA
                blit(buffer, screen,0,0,0,0,X_MAX,Y_MAX);


                /*Moviemnto*/

                //PAUSE
                if (key[KEY_ENTER])     //ENTER PARA PAUSAR, FICA ADICIONANDO REST.
                {
                    pause = load_bitmap("img/paused.bmp", 0);
                    draw_sprite(screen, pause, PAUSE_X, PAUSE_Y);
                    rest(500);
                    do
                    {
                        rest(50);
                    }while(!key[KEY_ENTER]);
                        rest(250);
                        destroy_bitmap(pause);
                }

                (jumping == 1) ? (pulex = 1) : (pulex = 0);


                if (key[KEY_SPACE]  && !jumping && !emptyflag)
                {
                    jumping = 1;
                    g = 1;
                    jump_index = 0;
                    walk_index = 0;
                    velY = -27;
                    play_sample( jump_sound, 255, 128, 1000, 0);
                }

                velX += ACCEL;
                bgx -= 4 + (int) velX; ;

                emptyflag  = 0;

                colision_state = Colide(&emptyflag, &g,&jhonny,emptyfields, fields_num, &velY, &walk_index, &jump_index, &pulex, &bgx, &jumping);

                switch(colision_state)
                {

                    case(NULO) :
                        jhonny.y += velY;
                        velY += GRAVITY;
                        break;
                    case(LOSS) :
                        play_sample(loss_sound, 255, 128, 2000, 0);
                        rest(100);
                        jhonny.lives--;
                        jhonny.y = Y_MAX - (JY + PY);
                        velY = 0;
                        g = 0;
                        jumping = 0;
                        emptyflag = 0;
                        walk_index = 0;
                        jump_index = 0;
                        jhonny.x = JX_I;
                        velX = 0;
                        bgx = 0;
                        break;
                    case(VERTICAL):
                        g = 0;
                        velY = 0;
                        jumping = 0;
                        break;
                };
                if (2*X_MAX + bgx < 0)
                {
                    bgx = 0;
                }

            }
            if(jhonny.lives == 0){
                free(emptyfields);
                destroy_bitmap(background);
                for(index=0;index<6;index++)
                    destroy_bitmap(walk[index]);
                for(index=0;index<19;index++)
                destroy_bitmap(jump[index]);
                destroy_bitmap(plataforma);
                destroy_bitmap(buffer);
                destroy_font(fontscore);
                destroy_sample(jump_sound);
                destroy_sample(loss_sound);
                Ranking(&jhonny.score);
            }
            jhonny.score = 0;
        break;
        case INSTRUCTION:
            rest(100);
            menu_state = INSTRUCTION;
            BITMAP *instructionbackground;
            instructionbackground = load_bitmap("menu/instructions.bmp", 0);
            draw_sprite(screen, instructionbackground, 0, 0);

            do{
                rest(10);
            }while(!key[KEY_ENTER]);

            destroy_bitmap(instructionbackground);

        break;
        case RANKING:
            rest(100);
            menu_state = RANKING;
            Ranking(&jhonny.score);
        break;
     };
    }while(menu_state != EXIT);
    deinit();
    return 0;
}
END_OF_MAIN();
