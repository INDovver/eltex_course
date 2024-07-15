#include <stdio.h>
#include <time.h>   
#include <stdlib.h> 
#include <string.h>


void str_to_char(char* str, unsigned char* ip){ //strtok получше понять
    char *token = strtok(str, ".");
    for(int i = 0; i<4;i++){
        ip[i] = atoi(token);
        token = strtok(NULL, "."); 
    }
}


void main(int argc, char* argv[]){
    srand(time(NULL));

    if (argc!=4){
        printf("Неверное количество аргументов");
        return;
    }

    int N = atoi(argv[3]);
    int count = 0;
    int subcount = 0;
    unsigned char ip_gate[4];
    unsigned char ip_mask[4];
    unsigned char ip_dist[N][4];
    
    str_to_char(argv[1],ip_gate);
    str_to_char(argv[2],ip_mask);
    
    for (int i = 0; i<N;i++){
        for (int j = 0; j<4;j++){
            ip_dist[i][j]=(rand()%256)&0b11111111; // узнать
        }
    }

    for (int i=0; i<N;i++){
        subcount = 0;
        for (int j = 0; j < 4; j++){
            if ((ip_gate[j]&ip_mask[j]^ip_dist[i][j]&ip_mask[j])==0){
                subcount++;
            }
        }
        if (subcount == 4){
            count++;
        }
    }
    printf("Количество узлов своей подсети: %d (%.2f%%)\n", count, (double)count / N * 100);
    printf("Количество узлов извне подсети: %d (%.2f%%)\n", N - count, (double)(N - count) / N * 100);
}