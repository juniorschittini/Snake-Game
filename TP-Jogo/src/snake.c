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



void StartBody(Game *g){
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
    g->food.pos = (Rectangle) {(float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), (float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), STD_SIZE_X, STD_SIZE_Y};
    g->food.color = FOOD_COLOR;
}

void StartRound(Game *g){
    StartBordas(g);
    StartBody(g);
    StartFood(g);
    g->time = GetTime();
}

void DrawBody(Game *g){
    DrawRectangleRec(g->body.pos, g->body.color);
    if(g->body.pos.x < 10 || g->body.pos.x + STD_SIZE_X > LARGURA - 10 ||
       g->body.pos.y < 10 || g->body.pos.y + STD_SIZE_Y > ALTURA - 10){
        //Se colidir com a borda, reStart o game
        StartRound(g);
    }
}

void DrawFood(Game *g){
    DrawRectangleRec(g->food.pos, g->food.color);
}

void DrawBordas(Game *g){
    //Draw as barreiras nas bordas
    for (int i = 0; i < 4; i++){
        DrawRectangleRec(g->bordas[i].pos, LIGHTGRAY);
    }
}

void Drawgame(Game *g){
    DrawBordas(g);
    DrawBody(g);
    DrawFood(g);
}

void Updatedirection(Game *g){
    //Update para qual direção a cobra vai  
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
    g->food.pos = (Rectangle) {(float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), 
        (float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), STD_SIZE_X, STD_SIZE_Y};

}

void UpdateRodada(Game *g){
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
    if (CheckCollisionRecs(g->body.pos, g->food.pos)){
        return 1;
    }
    return 0;
}
