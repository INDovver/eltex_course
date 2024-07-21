#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MQS 10
#define MF 100

typedef struct{
    char prior;
    char msg[MF];
}element;

typedef struct{
    element elinq[MQS];
    int size;
}queue;

void que_init(queue *que){
    que->size=0;
}

void add_el(queue *que){
    if (que->size==MQS){
        printf("Not enough space\n");
        return;
    }
    char text[MF];
    char pri;
    printf("Enter priority number [0:255] and message using comma\n");
    if (scanf("%d, %s", &pri, text)==2){
        while(getchar()!='\n');
        int i=0;
        for (; i<que->size&&que->elinq[i].prior<pri; i++);
        for(int j = que->size;j>i; j--) 
            que->elinq[j]= que->elinq[j-1];
        que->elinq[i].prior=pri;
        strcpy(que->elinq[i].msg, text);
        que->elinq[i].msg[MF-1] = '\0';
        que->size++;
        return;
    }
    while(getchar()!='\n');
    printf("Something goes wrong\n");
        return;
}

element first_el_extr(queue *que){
    if(que->size==0){
        printf("Queue is empty\n");
        element empty = {0, ""};
        return empty;
    } 
    element tmp = que->elinq[(que->size)-1];
    que->size--;
    printf("Message is: \"%s\", Prior number is: %d.\n", tmp.msg, tmp.prior);
    return tmp;
}

element min_extr_el(queue *que){
    element empty = {0, ""};
    if(que->size==0){
        printf("Queue is empty\n");
        return empty;
    }
    char prio; 
    printf("Enter element min prior number: ");
    if(scanf("%d", &prio)!=1){
        while(getchar()!='\n');
        printf("Wrong prior number\n");
        return empty;
    }
    while(getchar()!='\n');
    int cnt=0, i=0;
    for (;i<que->size; i++)
        if (que->elinq[i].prior>=prio){
            printf("Message is: \"%s\", Prior number is: %d.\n", que->elinq[i].msg, que->elinq[i].prior); 
            cnt++;
        }
    if (cnt==0){
        printf("Elements with given min prior numver haven't found\n");
        return empty;
    }
    element tmp = que->elinq[i];
    for (int n=i-cnt;n+cnt<que->size; n++){
        que->elinq[n]=que->elinq[n+cnt];
    }
    que->size-=cnt;
    return tmp;
}

element exact_extr_el(queue *que){
    element empty = {0, ""};
    if(que->size==0){
        printf("Queue is empty\n");
        return empty;
    }
    char prio; 
    printf("Enter element exact prior number: ");
    if(scanf("%d", &prio)!=1){
        while(getchar()!='\n');
        printf("Wrong prior number\n");
        return empty;
    }
    while(getchar()!='\n');
    int cnt=0, i=0;
    for (; que->elinq[i].prior<=prio&&i<que->size; i++)
        if (que->elinq[i].prior==prio){
            printf("Message is: \"%s\", Prior number is: %d.\n", que->elinq[i].msg, que->elinq[i].prior); 
            cnt++;
        }
    if (cnt==0){
        printf("Elements with given prior numver haven't found\n");
        return empty;
    }
    element tmp = que->elinq[i];
    for (int n=i-cnt;n+cnt<que->size; n++)
        que->elinq[n]=que->elinq[n+cnt];
    que->size-=cnt;
    return tmp;
}

int main(){
    queue queueue;
    que_init(&queueue);
    while (true){
        printf("Enter action key:\nA)Add\nE)Extract first element\nX)Extract elements by exact prior number\nM)Extract elements by minimum prior number\n");
        char c = getchar();
        c = tolower(c);
        while(getchar()!='\n');
        switch (c){
            case 'a': 
                add_el(&queueue);
                break; 
            case 'e':
                first_el_extr(&queueue);
                break;
            case 'x':
                exact_extr_el(&queueue);
                break;
            case 'm':
                min_extr_el(&queueue);
                break;
            case 'q': return 0;
            default: printf("Wrong key\n");
        }
    }
}