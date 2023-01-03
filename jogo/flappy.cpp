#include "raylib.h"
#include <stdlib.h>

class Bird { 
public:
    int x; 
    int y;
    float radius;
    Color color;
    bool state = true;
    float a = 0.35; //dinamica para pulo
    float b = 0.35;
    void draw() {
        DrawCircle(this->x, this->y, this->radius, this->color);
    }
    void mover(){
        a += 0.35;
        b = 7;
        this->y += a;
        if(this->y<10){
            y = 10;
            a = 0.35;
        }
    }
    void jump() { 
        b *= -1;
        a = b;
    }
    
}; 

class Column { //primeiro tipo de coluna
public:
    int x;
    const int y = 0; //coluna sempre vai começar da parte de cima da tela
    int width=80;
    int height= rand () % 440; //valor de altura sorteado
    
    void drawTopColumn() { //posicao x, posicao y
        DrawRectangle(this->x, this->y, this->width, this->height, GREEN);
}
    void drawUnderColumn() { //posicao x, posicao y somada a altura mais o espacamento
        DrawRectangle(this->x, this->y+height+135 , this->width, this->height+800, GREEN);
        
}
    void movement() {         
        this->x -= 2;
    }
    void draw(){
        drawTopColumn();   
        drawUnderColumn();
        movement();     
        if (this->x==-150){
            this-> x=1050;
            this-> height = rand () % 500;
        }
    }
};


class Clouds {
    public:
    int x;
    const int y = 50;
    float radiusH; //dimensoes da elipse (raio da altura e raio lateral)
    float radiusV;
    Color color; 
    void draw() {
        DrawEllipse(this->x, this->y, this->radiusH, this->radiusV, this->color);
    }
    void movement() {
        this-> x -= 1;
        if(this->x + 105 < 0) {
            this->x = 900;
        }    
    }
};


class Rules {
    public:
        
    bool CheckPositionOnScreen(int yBird, float radiusBird, int screenHeight) { //checar se "ultrapassou o chao" 
        if (yBird - radiusBird*2 > screenHeight + 10) {
        return true; }
        
        return false;
    }; 

    bool CheckCollision(int xBird, int yBird, int xColum, int yColum, int HeightColum) {
        int touchX = xBird + 10;  
        int touchY_top = yBird - 10; 
        int touchY_under = yBird + 10; 
        
        if (touchX == xColum)  { //colisao externa 
            if (yBird <= HeightColum) {
            return true; 
            }
            
            if (yBird >= HeightColum + 135) {
            return true;
            }  
        }
    
    if (xBird >= xColum && xBird <= xColum + 80) {
        if (touchY_top < HeightColum) {
            return true;
        }
        
        if (touchY_under  > HeightColum + 135) {
            return true;
        } 
    }    
    
    return false;
    }
    
};  

int main(void) { 
    const int screenWidth = 800;
    const int screenHeight = 600; 
    InitAudioDevice();
    Sound moveSound = LoadSound("move.wav");
    Sound dieSound = LoadSound("die.MP3");
    InitWindow(screenWidth, screenHeight, "Flappy Bird - S5/Informática");
    SetTargetFPS(60);
    
    
    Bird protagonist = {.x=100, .y=GetScreenHeight()/2, .radius=10
    , .color=YELLOW};
    Column colunas = {.x=800}; 
    Column c = {.x=1100};
    Column cc = {.x=1400};
    Column ccc = {.x=1700};
    Clouds first = {.x=200, .radiusH=100.0, .radiusV=20.00, .color=WHITE};
    Clouds second = {.x=700, .radiusH=100.0, .radiusV=20.00, .color=WHITE};
    
    int point = 0;
    Rules current; 
    
while (!WindowShouldClose()) {
            BeginDrawing();
if(protagonist.state == true) { //quando vivo:
            ClearBackground(BLUE);
            first.draw();
            first.movement();
            second.draw();
            second.movement();
            colunas.draw();
            c.draw();
            cc.draw();
            ccc.draw();
            protagonist.draw();
            if (IsKeyPressed(KEY_SPACE)) {
            protagonist.jump();
            PlaySound(moveSound);
            }            
            if(colunas.x==0){
            point += 1;
            first.draw();
            }
            if(c.x==0){
            point += 1; 
            }
            
            if(cc.x==0){
            point += 1;
            }
            
            if(ccc.x==0){
                point += 1;
            }
                
            
            DrawText(TextFormat("Score: %d", point),670,3,23,BLACK);
            
            if (current.CheckPositionOnScreen(protagonist.y, protagonist.radius, screenHeight) == true) {
                PlaySound(dieSound);
                protagonist.state = false;
            }
            
            if (current.CheckCollision(protagonist.x, protagonist.y, colunas.x, colunas.y, colunas.height) == true) {
                PlaySound(dieSound);
                protagonist.state = false;
                }
            
             if (current.CheckCollision(protagonist.x, protagonist.y, c.x, c.y, c.height) == true) {
                 PlaySound(dieSound);
                protagonist.state = false;
            }
             
            
            if (current.CheckCollision(protagonist.x, protagonist.y, cc.x, cc.y, cc.height) == true) {
                PlaySound(dieSound);
                protagonist.state = false;
            }
            
            
             if (current.CheckCollision(protagonist.x, protagonist.y, ccc.x, ccc.y, ccc.height) == true) {
                PlaySound(dieSound);
                protagonist.state = false;
            }
            
            protagonist.mover();
            EndDrawing();
}
            
            else if (protagonist.state == false) { //quando morto:
                DrawRectangle(0, 0, 800, 600, BLACK);
                DrawText(TextFormat("Sua pontuaçao: %d", point), 200, 250, 50, WHITE);
                DrawText("Reviva com qualquer tecla", 75, 300, 50, WHITE);
            if (IsKeyPressed(GetKeyPressed())) { //reiniciando as posicoes
                protagonist.x = 100;
                protagonist.y = GetScreenHeight()/2;
                colunas.x = 800; 
                c.x = 1100;
                cc.x = 1400;
                ccc.x = 1700;
                first.x =200;
                second.x = 700; 
                point = 0;
                protagonist.state = true;
                protagonist.jump();
            }  
            EndDrawing();
            }    

                        
        };   
    CloseWindow();   
    return 0;
} 