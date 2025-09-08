#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int vida;
    unsigned short int fase, R, G, B, contaClicks, lvClick, lvConta, lvDano;
    unsigned int coin;
} data;

int main() {
    data save[3];
    FILE *arquivo = fopen("savearch.dat", "wb");
    
    srand(time(NULL));
    
    for(int i = 0; i < 3; i++) {
        save[i].vida = 100;
        save[i].fase = 1;
        save[i].contaClicks = 1;
        save[i].lvClick = 1;
        save[i].lvConta = 1;
        save[i].lvDano = 1;
        save[i].R = rand() % 256;
        save[i].G = rand() % 256;
        save[i].B = rand() % 256;
        save[i].coin = 0;
    }

    fwrite(save, sizeof(data), 3, arquivo);
    fflush(arquivo);

    fclose(arquivo);
    return 0;
}