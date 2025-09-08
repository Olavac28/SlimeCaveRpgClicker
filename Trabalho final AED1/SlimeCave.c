#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//struct dos saves
typedef struct {
    int vida;
    unsigned short int fase, R, G, B, contaCliks, lvClick, lvConta, lvDano;
    unsigned int coin;
} data;
data save[3];

//struct do mouse
Vector2 mouse;

//structs dos botões
Rectangle save1but = {460, 500, 250, 85};
Rectangle save2but = {835, 650, 250, 85};
Rectangle save3but = {1210, 500, 250, 85};
Rectangle lojabut = {1550, 950, 340, 85};
Rectangle jogobut = {70, 950, 400, 85};
Rectangle amarelabut = {760, 640, 140, 140};
Rectangle vermelhabut = {760, 480, 140, 140};
Rectangle azulbut = {920, 480, 140, 140};
Rectangle Xbut = {1280, 80, 140, 140};
Rectangle up = {1200, 580, 100, 65};

void telaA(Texture2D fundosave);
void telaB(FILE *arquivo, Texture2D caveImg, Texture2D inimigo1, Texture2D inimigo2, Texture2D boss);
void telaC(Texture2D lojinha);
void telaD(FILE *arquivo, Texture2D lojinha, Texture2D BatataAmarela);
void telaE(FILE *arquivo, Texture2D lojinha, Texture2D BatataVermelha);
void telaF(FILE *arquivo, Texture2D lojinha, Texture2D refri);
void criabotaot1(Rectangle bot, float X, float Y, char texto[11]);
void criabotaot2(Rectangle bot, float X, float Y, char texto[3]);
void criabotaot3(Rectangle bot, float X, float Y, char texto[3]);
void savefile(FILE *arquivo);

//variáveis globais
short int logado;
char tela = 'a';

int main(void){
    InitWindow(1920, 1080, "Slime Cave");
    SetTargetFPS(60);
    srand(time(NULL));
    
    //abre o arquivo
    FILE *arquivo = fopen("savearch.dat", "rb+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    fread(save, sizeof(data), 3, arquivo);
    
    //structs das imagens
    Texture2D fundosave = LoadTexture("fundosave.png");
    Texture2D caveImg = LoadTexture("cave.png");
    Texture2D inimigo1 = LoadTexture("Cesao.png");
    Texture2D inimigo2 = LoadTexture("inimigo2.png");
    Texture2D boss = LoadTexture("boss.png");
    Texture2D lojinha = LoadTexture("maquina_de_vendas.png");
    Texture2D BatataAmarela = LoadTexture("BatataAmarela.png");
    Texture2D BatataVermelha = LoadTexture("BatataVermelha.png");
    Texture2D refri = LoadTexture("RefriAzul.png");
    
    //loop principal
    while(!WindowShouldClose()) {
        switch(tela) {
        case 'a':
            telaA(fundosave);
                break;
        case 'b':
            telaB(arquivo, caveImg, inimigo1, inimigo2, boss);
                break;
        case 'c':
            telaC(lojinha);
                break;
        case 'd':
            telaD(arquivo, lojinha, BatataAmarela);
                break;
        case 'e':
            telaE(arquivo, lojinha, BatataVermelha);
                break;
        case 'f':
            telaF(arquivo, lojinha, refri);
                break;
        }
    }
    
    UnloadTexture(caveImg);
    UnloadTexture(inimigo1);
    UnloadTexture(inimigo2);
    UnloadTexture(boss);
    UnloadTexture(lojinha);
    UnloadTexture(BatataAmarela);
    UnloadTexture(BatataVermelha);
    UnloadTexture(refri);
    fclose(arquivo);
    CloseWindow();
    return 0;
}

//tela dos saves
void telaA(Texture2D fundosave) {
        //verifica o click
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            mouse = GetMousePosition();
            if(CheckCollisionPointRec(mouse, save1but)) {
                logado = 0;
                tela = 'b';
            }
            if(CheckCollisionPointRec(mouse, save2but)) {
                logado = 1;
                tela = 'b';
            }
            if(CheckCollisionPointRec(mouse, save3but)) {
                logado = 2;
                tela = 'b';
            }
        }
        
        //desenha
        BeginDrawing();
            //fundo
            ClearBackground(DARKBLUE);
            DrawTextureEx(fundosave, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, WHITE);
            
            criabotaot1(save1but, save1but.x, save1but.y, "Save1");
            criabotaot1(save2but, save2but.x, save2but.y, "Save2");
            criabotaot1(save3but, save3but.x, save3but.y, "Save3");
        EndDrawing();
}

//tela do jogo
void telaB(FILE *arquivo, Texture2D caveImg, Texture2D inimigo1, Texture2D inimigo2, Texture2D boss) {
    //verifica os clicks 
    mouse = GetMousePosition();
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if(CheckCollisionPointRec(mouse, lojabut))
            tela = 'c';
        else if(save[logado].contaCliks >= 50 - (save[logado].lvConta - 1) * 5) {
            save[logado].vida -= (10 + (save[logado].lvClick - 1) * 2) * (20 + (save[logado].lvDano - 1) * 20);
            save[logado].contaCliks = 1;
        }
        else {
            save[logado].vida -= 10 + (save[logado].lvClick - 1) * 2;
            save[logado].contaCliks++;
        }
    }
    
    //condicionais de morte
    if(save[logado].vida <= 0) {
        if(save[logado].fase % 10 == 0) {
            save[logado].coin += 10 * save[logado].fase;
            save[logado].fase++;
        }
        else if(save[logado].fase % 3 == 0) {
            save[logado].coin += 3 * save[logado].fase;
            save[logado].fase++;
        }
        else {
            save[logado].coin += 1 * save[logado].fase;
            save[logado].fase++;
        }
        
        save[logado].R = rand() % 256;
        save[logado].G = rand() % 256;
        save[logado].B = rand() % 256;
        
        if(save[logado].fase % 10 == 0)
            save[logado].vida = 100 + (save[logado].fase * 70);
        else if(save[logado].fase % 3 == 0)
            save[logado].vida = 100 + (save[logado].fase * 15);
        else
            save[logado].vida = 100 + ((save[logado].fase - 1) * 10);
    }
    
    savefile(arquivo);
    
    //desenha
    BeginDrawing();
        //fundo
        ClearBackground(DARKBLUE);
        DrawTextureEx(caveImg, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, WHITE);
        
        //mostra as variáveis
        DrawText(TextFormat("Fase: %hu", save[logado].fase), 5, 5, 60, LIME);
        DrawText(TextFormat("Coin: %u", save[logado].coin), 5, 70, 60, LIME);
        
        //mostra o bicho
        if(save[logado].fase % 10 == 0)
            DrawTextureEx(boss, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, (Color){save[logado].R, save[logado].G, save[logado].B, 255});
        else if(save[logado].fase % 3 == 0)
            DrawTextureEx(inimigo2, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, (Color){save[logado].R, save[logado].G, save[logado].B, 255});
        else
            DrawTextureEx(inimigo1, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, (Color){save[logado].R, save[logado].G, save[logado].B, 255});
        
        //mostra a vida do bicho
        DrawText(TextFormat("Vida: %d", save[logado].vida), 1250, 350, 60, LIME);
        
        //botão para a loja
        criabotaot1(lojabut, lojabut.x, lojabut.y, "Lojinha ->");
    EndDrawing();
}

//tela da loja
void telaC(Texture2D lojinha) {
    //verifica os clicks
    mouse = GetMousePosition();
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if(CheckCollisionPointRec(mouse, jogobut))
            tela = 'b';
        else if(CheckCollisionPointRec(mouse, amarelabut))
            tela = 'd';
        else if(CheckCollisionPointRec(mouse, vermelhabut))
            tela = 'e';
        else if(CheckCollisionPointRec(mouse, azulbut))
            tela = 'f';
    }
    
    //desenha
    BeginDrawing();
        //fundo
        ClearBackground(DARKBLUE);
        DrawTextureEx(lojinha, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, WHITE);
        
        //verifica as ações com o mouse
        if(CheckCollisionPointRec(mouse, amarelabut))
            DrawRectangleRec(amarelabut, (Color){255, 255, 255, 150});
        if(CheckCollisionPointRec(mouse, vermelhabut))
            DrawRectangleRec(vermelhabut, (Color){255, 255, 255, 150});
        if(CheckCollisionPointRec(mouse, azulbut))
            DrawRectangleRec(azulbut, (Color){255, 255, 255, 150});
        
        //botao para o jogo
        criabotaot1(jogobut, jogobut.x, jogobut.y, "<- Caverna");
        
        //dinheiro na conta
        DrawText(TextFormat("Coin: %d",save[logado].coin), 5, 5, 60, WHITE);
    EndDrawing();
}

//tela da batata amarela
void telaD(FILE *arquivo, Texture2D lojinha, Texture2D BatataAmarela) {
    //verifica os clicks
    mouse = GetMousePosition();
    int preco = 300 + (save[logado].lvDano - 1) * 300;
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(CheckCollisionPointRec(mouse, Xbut))
            tela = 'c';
        if(CheckCollisionPointRec(mouse, up)) {
            if(save[logado].coin >= preco) {
                save[logado].lvDano++;
                save[logado].coin -= preco;
                savefile(arquivo);
            }
        }
    }
    
    //desenha
    BeginDrawing();
        //fundo
        ClearBackground(DARKBLUE);
        DrawTextureEx(lojinha, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, WHITE);
        DrawRectangleRec((Rectangle){0, 0, 1960, 1080}, (Color){0, 0, 0, 150});
        
        //verifica as ações com o mouse
        if(CheckCollisionPointRec(mouse, Xbut))
            DrawRectangleRec(Xbut, (Color){150, 0, 0, 150});
        
        //desenha a batata
        DrawTextureEx(BatataAmarela, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, WHITE);
        
        //desenha o texto de compra
        DrawText(TextFormat("Aumentar o dano da\nhabilidade: lv %d -> lv %d", save[logado].lvDano, save[logado].lvDano + 1), 1100, 320, 60, WHITE);
        DrawText(TextFormat("Preço: %d", preco), 1100, 480, 60, WHITE);
        DrawText(TextFormat("Coin: %d",save[logado].coin), 5, 5, 60, WHITE);
        
        //desenha o botão
        if(save[logado].coin >= preco)
            criabotaot3(up, up.x, up.y, "up");
        else
            criabotaot2(up, up.x, up.y, "up");
    EndDrawing();
}

//tela da batata vermelha
void telaE(FILE *arquivo, Texture2D lojinha, Texture2D BatataVermelha) {
    //verifica os clicks
    mouse = GetMousePosition();
    int preco = 200 + (save[logado].lvConta - 1) * 200 * (save[logado].lvConta - 2);
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(CheckCollisionPointRec(mouse, Xbut))
            tela = 'c';
        if(CheckCollisionPointRec(mouse, up)) {
            if(save[logado].coin >= preco && save[logado].lvConta < 9) {
                save[logado].lvConta++;
                save[logado].coin -= preco;
                savefile(arquivo);
            }
        }
    }
    
    //desenha
    BeginDrawing();
        //fundo
        ClearBackground(DARKBLUE);
        DrawTextureEx(lojinha, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, WHITE);
        DrawRectangleRec((Rectangle){0, 0, 1960, 1080}, (Color){0, 0, 0, 150});
        
        //verifica as ações com o mouse
        if(CheckCollisionPointRec(mouse, Xbut))
            DrawRectangleRec(Xbut, (Color){150, 0, 0, 150});
        
        //desenha a batata
        DrawTextureEx(BatataVermelha, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, WHITE);
        
        //desenha o texto de compra
        if(save[logado].lvConta < 9) {
            DrawText(TextFormat("Aumentar o uso da\nhabilidade: lv %d -> lv %d", save[logado].lvConta, save[logado].lvConta + 1), 1100, 320, 60, WHITE);
            DrawText(TextFormat("Preço: %d", preco), 1100, 480, 60, WHITE);
        }
        else
            DrawText("lv MAX", 1100, 480, 60, WHITE);
        DrawText(TextFormat("Coin: %d",save[logado].coin), 5, 5, 60, WHITE);
        
        //desenha o botão
        if(save[logado].lvConta < 9) {
            if(save[logado].coin >= preco)
                criabotaot3(up, up.x, up.y, "up");
            else
                criabotaot2(up, up.x, up.y, "up");
        }
        else
            criabotaot2(up, up.x, up.y, "up");
    EndDrawing();
}

//tela do refrigerante azul
void telaF(FILE *arquivo, Texture2D lojinha, Texture2D refri) {
    //verifica os clicks
    mouse = GetMousePosition();
    int preco = 200 + (save[logado].lvClick - 1) * 200;
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(CheckCollisionPointRec(mouse, Xbut))
            tela = 'c';
        if(CheckCollisionPointRec(mouse, up)) {
            if(save[logado].coin >= preco) {
                save[logado].lvClick++;
                save[logado].coin -= preco;
                savefile(arquivo);
            }
        }
    }
    
    //desenha
    BeginDrawing();
        //fundo
        ClearBackground(DARKBLUE);
        DrawTextureEx(lojinha, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, WHITE);
        DrawRectangleRec((Rectangle){0, 0, 1960, 1080}, (Color){0, 0, 0, 150});
        
        //verifica as ações com o mouse
        if(CheckCollisionPointRec(mouse, Xbut))
            DrawRectangleRec(Xbut, (Color){150, 0, 0, 150});
        
        //desenha o refri
        DrawTextureEx(refri, (Vector2){0.0f, 0.0f}, 0.0f, 20.0f, WHITE);
        
        //desenha o texto de compra
        DrawText(TextFormat("Upar o click: lv %d -> lv %d", save[logado].lvClick, save[logado].lvClick + 1), 1100, 380, 60, WHITE);
        DrawText(TextFormat("Preço: %d", preco), 1100, 480, 60, WHITE);
        DrawText(TextFormat("Coin: %d",save[logado].coin), 5, 5, 60, WHITE);
        
        //desenha o botão
        if(save[logado].coin >= preco)
            criabotaot3(up, up.x, up.y, "up");
        else
            criabotaot2(up, up.x, up.y, "up");
    EndDrawing();
}

//função para criar botão do tipo 1
void criabotaot1(Rectangle bot, float X, float Y, char texto[11]) {
    DrawRectangleRec(bot, LIGHTGRAY);
    DrawRectangleLinesEx(bot, 4, GRAY);
    DrawText(texto, X + 35, Y + 15, 60, DARKGRAY);
}

//função para criar botão do tipo 2
void criabotaot2(Rectangle bot, float X, float Y, char texto[3]) {
    DrawRectangleRec(bot, DARKBLUE);
    DrawRectangleLinesEx(bot, 4, BLACK);
    DrawText(texto, X + 15, Y, 60, GRAY);
}

//função para criar botão do tipo 3
void criabotaot3(Rectangle bot, float X, float Y, char texto[3]) {
    DrawRectangleRec(bot, DARKBLUE);
    DrawRectangleLinesEx(bot, 4, BLACK);
    DrawText(texto, X + 15, Y, 60, WHITE);
}

//salva no arquivo
void savefile(FILE *arquivo) {
    rewind(arquivo); //volta
    fwrite(save, sizeof(data), 3, arquivo);
    fflush(arquivo);
}