#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

#define FILE_NAME "textfile.txt"
#define KEY_ID 12
#define MAX_NUM 10+1
#define SIZE_NUM sizeof(int)*(MAX_NUM+2)

sig_atomic_t go_on;


void sig_act(int signum){
    go_on=1;
}


int max(int *array, int size){
    int tmp = 0;
    for(int i=1;i<size;i++){
        if(array[i]>array[tmp])
            tmp=i;
    }
    return array[tmp];
}


int min(int *array, int size){
    int tmp = 0;
    for(int i=1;i<size;i++){
        if(array[i]<array[tmp])
            tmp=i;
    }
    return array[tmp];
}


void sem_shm_get(int* semid, int* shmid, size_t size){
    key_t ipc_key = ftok(FILE_NAME, KEY_ID);
    if (ipc_key ==-1){
        perror("key error");
        exit(EXIT_FAILURE);
    }
    *shmid = shmget(ipc_key,size,IPC_CREAT|0666);
    if (*shmid ==-1){
        perror("shared memory error");
        exit(EXIT_FAILURE);
    }
    *semid = semget(ipc_key,2,IPC_CREAT|0666);
    if (*semid ==-1){
        perror("semaphore error");
        shmctl(*shmid,IPC_RMID,NULL);
        exit(EXIT_FAILURE);
    }
}

void parent_func(void){
    int semid;
    int shmid;
    int cntr=0;
    sem_shm_get(&semid, &shmid,SIZE_NUM);
    int* ptr = shmat(shmid,NULL,0);

    while(go_on==0){
        int num_of_num = rand()%MAX_NUM; //генерация количества чисел
        size_t size = sizeof(int)*(num_of_num+3); //размер с учётом доп. элементов (ниже рассписывается)
        int* nums = malloc(size); //создание динамического массива
        (*nums)=num_of_num; //в первый элемент записывается количество сгенерированных чисел
        for(int i=1; i<=num_of_num;i++){
            nums[i]=rand()%MAX_NUM;   //заполнение массива случайными числами (последние два элемента остаются пустыми(ниже рассписывается почему))
            printf("%d ",nums[i]);
        }
        printf("\n");  
        semop(semid,&(struct sembuf){0,0,0},1); //ожидание разрешения копирования в разделяемую память (изначально равно нулю, так что сразу срабатывает)
        memcpy(ptr,nums,size); //копирывание в разделяемую память
        free(nums);
        semop(semid,&(struct sembuf){0,1,0},1); //разрешение обращения к разделяемой памяти для доч. процесса
        semop(semid,&(struct sembuf){1,-1,0},1); //ожидание разрешения считывания с разделяемой памяти для род.процесса
        printf("max num is %d and min num is %d\n",ptr[num_of_num+1],ptr[num_of_num+2]);
        cntr++;
        sleep(1);
    }
    printf("\nnum of processed data sets: %d\n",cntr);
    shmdt((void*)ptr);
    shmctl(shmid,IPC_RMID,NULL);
    semctl(semid,0,IPC_RMID);
}


void child_func(void){
    int semid;
    int shmid;
    sem_shm_get(&semid, &shmid,SIZE_NUM);
    int* ptr = shmat(shmid,NULL,0);
    while(go_on==0){
        semop(semid,&(struct sembuf){0,-1,0},1); //ожидание разрешения обращения к разделяемой памяти
        ptr[(*ptr)+1]=max(ptr+1,*ptr); //записывает в предпоследний элемент массива max число
        ptr[(*ptr)+2]=min(ptr+1,*ptr); //записывает в последний элемент массива min число
        semop(semid,&(struct sembuf){1,1,0},1); //разрешение на считывание разделяемой памяти для род. процесса

    }
    shmdt((void*)ptr);
    exit(EXIT_SUCCESS);
}


int main(){
    srand(time(NULL));
    signal(SIGINT,sig_act);
    pid_t pid = fork();
    switch (pid){
        case -1:
            perror("fork error"); 
            exit(EXIT_FAILURE);
        case 0:
            child_func();
        default:
            parent_func();
            wait(NULL);
            exit(EXIT_SUCCESS);
    }
}