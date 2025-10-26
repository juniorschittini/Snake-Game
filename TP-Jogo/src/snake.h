#ifndef SNAKE_H
#define SNAKE_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define LARGURA 660
#define ALTURA 660
#define STD_SIZE_X 40
#define STD_SIZE_Y 40
#define TIME 0.2
#define COOLDOWN 0.2
#define SNAKE_COLOR YELLOW
#define FOOD_COLOR BLUE

typedef struct Bordas{
    Rectangle pos;
}Bordas;

typedef struct Body Body;

typedef struct Body{
    Rectangle pos;
    Color color;
    int direction;
    Body *next;
    Body *prev;
}Body;

typedef struct Snake {
    Body *head;
    Body *tail;
    int length;
} Snake;

typedef struct Food{
    Rectangle pos;
    Color color;
}Food;

typedef struct Game{
    Body body;
    Snake snake;
    Food food;
    Bordas bordas[4];
    double time;
    double cooldown;
}Game;

Body* CreatSegment (float x, float y, Color color, Rectangle pos);
void StartBody(Game *g);
void StartBordas(Game *g);
void StartFood(Game *g);
void StartRound(Game *g);
void MoveGrowSnake(Game *g, int, int, bool);
void DrawBody(Game *g);
void DrawFood(Game *g);
void DrawBordas(Game *g);
void DrawHomeScreen(Game *g);
void DrawFinalScreen(Game *g);
void Drawgame(Game *g);
void Updatedirection(Game *g);
void UpdatePosBody(Game *g);
void UpdatePosFood(Game *g);
void UpdateRodada(Game *g);
int CollisionFood(Game *g);

#endif /* SNAKE_H */
