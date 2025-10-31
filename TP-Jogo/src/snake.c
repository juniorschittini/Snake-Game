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

int gameOver = 1;
int HomeScreen = 1;
int FinalScreen = 0;
int ganhou = 0;
int perdeu = 0;
int not_grow = true;

Body* CreatSegment (float x, float y, Color color, Rectangle pos){
    Body* segment = (Body*)malloc(sizeof(Body));
    segment->pos = (Rectangle){x, y, STD_SIZE_X, STD_SIZE_Y};
    segment->color = color;
    segment->next = NULL;
    segment->prev = NULL;
    return segment;
}

void StartBody(Game *g){
    // Inicia a posição, direção e cor da cobra
    g->body.pos = (Rectangle) {LARGURA/2 - STD_SIZE_X, ALTURA - STD_SIZE_Y -10, STD_SIZE_X, STD_SIZE_Y};
    g->body.direction = 0;
    g->body.color = SNAKE_COLOR;
    // Cria o primeiro segmento (cabeça) usando a posição recém-definida
    Body* head = CreatSegment(g->body.pos.x, g->body.pos.y, g->body.color, g->body.pos);
    g->snake.head = head;
    g->snake.tail = head;
    g->snake.length = 1;
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
    //Inicia uma nova rodada: libera qualquer snake anterior, depois inicializa
    FreeSnake(g);
    StartBordas(g);
    StartBody(g);
    StartFood(g);
    g->time = GetTime();
}

// Libera todos os segmentos da cobra (lista encadeada)
void FreeSnake(Game *g){
    if (g == NULL) return;
    Body *cur = g->snake.head;
    while (cur != NULL) {
        Body *next = cur->next;
        free(cur);
        cur = next;
    }
    g->snake.head = NULL;
    g->snake.tail = NULL;
    g->snake.length = 0;
}

// Função principal de movimento e crescimento
void MoveGrowSnake(Game *g, int novo_x, int novo_y, bool not_grow) {
    // 1. Cria a nova Cabeça (Inserção no Início)
    
    Body *new_head = CreatSegment (novo_x, novo_y, g->body.color, g->body.pos);

    // Conecta o novo segmento na frente da antiga cabeça
    new_head->next = g->snake.head;
    g->snake.head->prev = new_head;
    
    // Atualiza a cabeça da cobra
    g->snake.head = new_head;;
    g->snake.length++;

    // 2. Remove a Cauda (Se não houve crescimento)
    if (not_grow) {
        Body *remove_tail = g->snake.tail;
        
        // O nó anterior à cauda passa a ser a nova cauda
        Body *new_tail = remove_tail->prev;
        
        // Remove a ligação para o nó antigo da cauda
        if (new_tail != NULL) {
            new_tail->next = NULL;
            g->snake.tail = new_tail;
            g->snake.length--;
            
            // Libera a memória do segmento removido
            free(remove_tail);
        }
        // Caso especial: se a cobra tem 1 segmento (não deveria acontecer no loop do jogo, mas é boa prática)
        else {
            // Se só tem 1 segmento, ele está sendo removido. A lista fica vazia.
            g->snake.head = NULL;
            g->snake.tail = NULL;
            g->snake.length = 0;
            free(remove_tail);
        }
    }
    // Se nao_crescer for 'false', a cauda não é removida, e a cobra cresce 1 segmento!
}

// Verifica se a cobra colidiu com seu próprio corpo
bool CheckSelfCollision(Game *g) {
    if (g->snake.head == NULL || g->snake.head->next == NULL) return false;
    
    // Pega a posição da cabeça
    Rectangle headPos = g->snake.head->pos;
    
    // Começa do segundo segmento (pula a cabeça)
    Body *cur = g->snake.head->next;
    
    // Verifica colisão com cada segmento do corpo
    while (cur != NULL) {
        if (CheckCollisionRecs(headPos, cur->pos)) {
            return true;
        }
        cur = cur->next;
    }
    
    return false;
}

void DrawBody(Game *g){
    // Percorre a lista de segmentos e desenha cada um
    Body *cur = g->snake.head;
    while (cur != NULL) {
        DrawRectangleRec(cur->pos, cur->color);
        cur = cur->next;
    }
    if (g->snake.length >= 26){
        gameOver = 0;
        FinalScreen = 1;
        ganhou = 1;
    }
    // Verifica colisão da cabeça com as bordas e com o próprio corpo
    if (g->snake.head != NULL) {
        Rectangle headPos = g->snake.head->pos;
        if(headPos.x < 10 || headPos.x + STD_SIZE_X > LARGURA - 10 ||
           headPos.y < 10 || headPos.y + STD_SIZE_Y > ALTURA - 10 ||
           CheckSelfCollision(g)){ 
            // Se a cobra sair da área de jogo ou colidir consigo mesma, sinaliza fim de jogo
            gameOver = 0;
            FinalScreen = 1;
            perdeu = 1;
        }
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
    DrawText ("SNAKE GAME", 165, 100, 50, GREEN);
    DrawText("Hit - Enter - ", 250, 600, 20, WHITE);
    if (IsKeyPressed(KEY_ENTER)){
        HomeScreen = 0;
        StartRound(g);
    }
}

void DrawFinalScreen(Game *g){
    //Desenha a tela final do jogo
    if (gameOver == 0 && ganhou == 1) {
        DrawText("YOU WIN!", 150, 230, 80, GOLD);
        DrawText("Hit - Enter - ", 250, 600, 20, WHITE);
            if (IsKeyPressed(KEY_ENTER)){
                gameOver = 1;   
                FinalScreen = 0;
                ganhou = 0;
                StartRound(g);}
    } if ( gameOver == 0 && perdeu == 1) {
        DrawText("YOU LOOSE!", 105, 230, 80, RED);
        DrawText("Hit - Enter - ", 250, 600, 20, WHITE);
        //Se o jogador apertar enter, reinicia a rodada
        if (IsKeyPressed(KEY_ENTER)){
            //atualiza o jogo para reiniciar a rodada
            gameOver = 1;
            FinalScreen = 0;
            perdeu = 0;
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
    // Calcula a nova posição da cabeça com base na direção atual
    if (g->snake.head == NULL) return;

    int new_x = (int)g->snake.head->pos.x;
    int new_y = (int)g->snake.head->pos.y;

    if (g->body.direction == 0){
        new_y -= STD_SIZE_Y;
    }
    if (g->body.direction == 1){
        new_x += STD_SIZE_X;
    }
    if (g->body.direction == 2){
        new_y += STD_SIZE_Y;
    }
    if (g->body.direction == 3){
        new_x -= STD_SIZE_X;
    }

    // Cria retângulo temporário para checar colisão com a comida
    Rectangle newHeadRect = (Rectangle){(float)new_x, (float)new_y, STD_SIZE_X, STD_SIZE_Y};

    // Se a nova posição colidir com a comida, não remover a cauda (cresce)
    bool willGrow = CheckCollisionRecs(newHeadRect, g->food.pos);

    // Move/grow a cobra usando a função que gerencia lista encadeada
    MoveGrowSnake(g, new_x, new_y, !willGrow);

    // Se cresceu (colidiu com comida), gera nova comida
    if (willGrow) {
        UpdatePosFood(g);
    }
}

void UpdatePosFood(Game *g){
    //Gera uma nova posição para a comida
    g->food.pos = (Rectangle) {(float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), 
        (float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), STD_SIZE_X, STD_SIZE_Y};
}

void UpdateRodada(Game *g){
    // Atualiza o estado do jogo a cada frame
    DrawText(TextFormat("Score: %d", g->snake.length - 1), 10, 10, 20, WHITE);
    Updatedirection(g);
    if (GetTime() - g->time > TIME){
        UpdatePosBody(g);
        g->time = GetTime();
        g->cooldown = COOLDOWN;
    }
}

int CollisionFood(Game *g){
    // Verifica se a cabeça da cobra colidiu com a comida
    if (g->snake.head != NULL && CheckCollisionRecs(g->snake.head->pos, g->food.pos)){
        return 1;
    }
    return 0;
}
