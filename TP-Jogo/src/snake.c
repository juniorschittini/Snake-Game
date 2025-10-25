#include "raylib.h"
#include "snake.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define LARGURA 660
#define ALTURA 660
#define STD_SIZE_X 40
#define STD_SIZE_Y 40
#define TIME 0.2
#define COOLDOWN 0.2
#define SNAKE_COLOR YELLOW
#define FOOD_COLOR BLUE

int gameOver = 1;
int HomeScreen = 1;
int FinalScreen = 0;
int ganhou = 0;
int perdeu = 0;

void StartBody(Game *g){
    //Inicia a posição, direção e cor da cobra
    g->body.pos = (Rectangle) {LARGURA/2 - STD_SIZE_X, ALTURA - STD_SIZE_Y -10, STD_SIZE_X, STD_SIZE_Y};
    g->body.direction = 0;
    g->body.color = SNAKE_COLOR;
    }

void StartBordas(Game *g){
    //Borda de cima
    g->bordas[0].pos = (Rectangle) {0, 0, LARGURA, 10};
    //Borda da direita
    g->bordas[1].pos = (Rectangle) {LARGURA - 10, 0, 10, ALTURA};
    //Borda de baixo
    g->bordas[2].pos = (Rectangle) {0, ALTURA - 10, LARGURA, 10};
    //Borda da esquerda
    g->bordas[3].pos = (Rectangle) {0, 0, 10, ALTURA};
}



void StartFood(Game *g){
    //Gera uma posição aleatória para a comida
    g->food.pos = (Rectangle) {(float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), (float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), STD_SIZE_X, STD_SIZE_Y};
    g->food.color = FOOD_COLOR;
}

void StartRound(Game *g){
    //Inicia uma nova rodada
    StartBordas(g);
    StartBody(g);
    StartFood(g);
    g->time = GetTime();
}

void DrawBody(Game *g){
    DrawRectangleRec(g->body.pos, g->body.color);
    if(g->body.pos.x < 10 || g->body.pos.x + STD_SIZE_X > LARGURA - 10 ||
       g->body.pos.y < 10 || g->body.pos.y + STD_SIZE_Y > ALTURA - 10){
        //Se a cobra sair da área de jogo, reinicia o jogo
        gameOver = 0;
        FinalScreen = 1;
        perdeu = 1;
    }
}

void DrawFood(Game *g){
    //Desenha a comida so se o jogo não tiver acabado
    if(gameOver != 0){
        DrawRectangleRec(g->food.pos, g->food.color);
    }
}


void DrawBordas(Game *g){
    //Desenha as barreiras nas bordas
    for (int i = 0; i < 4; i++){
        DrawRectangleRec(g->bordas[i].pos, LIGHTGRAY);
    }
}

void DrawHomeScreen(Game *g){
    //Desenha a tela inicial do jogo
    DrawText("SNAKE GAME", 165, 100, 50, GREEN);
    DrawText("PRESS \"ENTER\" TO START", 180, 400, 20, WHITE);
    if (IsKeyPressed(KEY_ENTER)){
        HomeScreen = 0;
        StartRound(g);
    }
}

void DrawFinalScreen(Game *g){
    //Desenha a tela final do jogo
    if (gameOver == 0 && ganhou == 1) {
        DrawText("YOU WIN!", 200, 200, 80, GOLD);
        DrawText("PRESS \"ENTER\" TO PLAY AGAIN", 150, 500, 20, WHITE);
            if (IsKeyPressed(KEY_ENTER)){
                FinalScreen = 0;
                StartRound(g);}
    } if ( gameOver == 0 && perdeu == 1) {
        DrawText("YOU LOOSE!", 105, 230, 80, RED);
        DrawText("PRESS \"ENTER\" TO CONTINUE", 165, 600, 20, WHITE);
        //Se o jogador apertar enter, reinicia a rodada
        if (IsKeyPressed(KEY_ENTER)){
            //atualiza o jogo para reiniciar a rodada
            gameOver = 1;
            StartRound(g);
        } 
    }  
}

void Drawgame(Game *g){
    //Desenha o jogo
    DrawBordas(g);
    if (HomeScreen){
        DrawHomeScreen(g);
        return;
    }
    if (FinalScreen){
        DrawFinalScreen(g);
        return;
    }
    DrawBody(g);
    DrawFood(g);
}

void Updatedirection(Game *g){
    //Atualiza para qual direção a cobra vai  
    if(IsKeyDown(KEY_UP) && g->body.direction != 2 && GetTime() - g->cooldown > COOLDOWN){
        g->body.direction = 0;
        g->cooldown = GetTime();
    }
    if(IsKeyDown(KEY_RIGHT) && g->body.direction != 3 && GetTime() - g->cooldown > COOLDOWN){
        g->body.direction = 1;
        g->cooldown = GetTime();
    }
    if(IsKeyDown(KEY_DOWN) && g->body.direction != 0 && GetTime() - g->cooldown > COOLDOWN){
        g->body.direction = 2;
        g->cooldown = GetTime();
    }
    if(IsKeyDown(KEY_LEFT) && g->body.direction != 1 && GetTime() - g->cooldown > COOLDOWN){
        g->body.direction = 3;
        g->cooldown = GetTime();
    }
}



void UpdatePosBody(Game *g){
    // Atualiza a posição da cobra conforme a direção
    if (g->body.direction == 0){
        g->body.pos.y -= STD_SIZE_Y;
    }
    if (g->body.direction == 1){
        g->body.pos.x += STD_SIZE_X;
    }
    if (g->body.direction == 2){
        g->body.pos.y += STD_SIZE_Y;
    }
    if (g->body.direction == 3){
        g->body.pos.x -= STD_SIZE_X;
    }
}

void UpdatePosFood(Game *g){
    //Gera uma nova posição para a comida
    g->food.pos = (Rectangle) {(float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), 
        (float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), STD_SIZE_X, STD_SIZE_Y};

}

void UpdateRodada(Game *g){
    // Atualiza o estado do jogo a cada frame
    Updatedirection(g);
    if (GetTime() - g->time > TIME){
        UpdatePosBody(g);
        g->time = GetTime();
        g->cooldown = COOLDOWN;
    }
    if(CollisionFood(g)){
        UpdatePosFood(g);
        DrawBody(g);
    }
}

int CollisionFood(Game *g){
    //Verifica se a cobra colidiu com a comida
    if (CheckCollisionRecs(g->body.pos, g->food.pos)){
        return 1;
    }
    return 0;
}
