#include "raylib.h"
#include "snake.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define LARGURA 660
#define ALTURA 660
#define STD_SIZE_X 40
#define STD_SIZE_Y 40
#define TIME 0.2
#define COOLDOWN 0.2
#define SNAKE_COLOR YELLOW
#define FOOD_COLOR BLUE



int main(){
    Game game = {0};
    int gameOver = 1;
    InitWindow(LARGURA, ALTURA, "Snake Game");
    SetTargetFPS(60);
    srand(time(NULL));
    
    StartRound(&game);
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        if (gameOver){
            Drawgame(&game);
            UpdateRodada(&game);
            if (CollisionFood(&game)){
                gameOver = 1;
            }
        } 
        EndDrawing();
    }
    FreeSnake(&game);
    CloseWindow();
    return 0;
}
