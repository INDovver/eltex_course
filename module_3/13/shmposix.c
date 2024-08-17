#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <fcntl.h>

#define SEM1_NAME "/semaphore1"
#define SEM2_NAME "/semaphore2"
#define SHM_NAME "/sharedmemory"
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


void sem_shm_get(sem_t *semid[], int* shmid,size_t size){
    semid[0] = sem_open(SEM1_NAME,O_CREAT,0666,0);
    if (semid[0]==SEM_FAILED){
        perror("semaphore 1 error");
        exit(EXIT_FAILURE);
    }   
    semid[1] = sem_open(SEM2_NAME,O_CREAT,0666,0);
    if (semid[1]==SEM_FAILED){
        perror("semaphore 2 error");
        sem_close(semid[0]);
        sem_unlink(SEM1_NAME);
        exit(EXIT_FAILURE);
    }
    *shmid = shm_open(SHM_NAME,O_RDWR|O_CREAT,0666);
    if (*shmid==-1){
        sem_close(semid[0]);
        sem_unlink(SEM1_NAME);                      
        sem_close(semid[1]);
        sem_unlink(SEM2_NAME);
        perror("shared memory error");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(*shmid, size) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
}

void parent_func(void){
    sem_t* semid[2];
    int shmid;
    int cntr=0;
    sem_shm_get(semid, &shmid,SIZE_NUM);
    int* ptr = mmap(NULL,SIZE_NUM,PROT_READ|PROT_WRITE,MAP_SHARED,shmid,0);
    if (ptr == MAP_FAILED) {
        perror("mmap error");
        exit(EXIT_FAILURE);
    }

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
        memcpy(ptr,nums,size); //копирывание в разделяемую память
        free(nums);
        sem_post(semid[0]);
        sem_wait(semid[1]);
        printf("max num is %d and min num is %d\n",ptr[num_of_num+1],ptr[num_of_num+2]);
        cntr++;
        sleep(1);
    }
    printf("\nnum of processed data sets: %d\n",cntr);
    munmap(ptr,SIZE_NUM);
    shm_unlink(SHM_NAME);
    sem_close(semid[0]);
    sem_unlink(SEM1_NAME);
    sem_close(semid[1]);
    sem_unlink(SEM2_NAME);
}


void child_func(void){
    sem_t* semid[2];
    int shmid;
    sem_shm_get(semid, &shmid,SIZE_NUM);
    int* ptr = mmap(NULL,SIZE_NUM,PROT_READ|PROT_WRITE,MAP_SHARED,shmid,0);
    if (ptr == MAP_FAILED) {
        perror("mmap error");
        exit(EXIT_FAILURE);
    }
    while(go_on==0){
        if(sem_trywait(semid[0])==-1){
            usleep(1e3); //Добавил trywait, так как возникает бесконечное блокирование, т.к. род. процесс не разблокируя семафор exit'ается
            continue; //В сис5 такой проблемы не возникает, почему-то, но в посиксе такая проблема есть!
        } 
        ptr[(*ptr)+1]=max(ptr+1,*ptr); //записывает в предпоследний элемент массива max число
        ptr[(*ptr)+2]=min(ptr+1,*ptr); //записывает в последний элемент массива min число
        sem_post(semid[1]);
    }
    munmap(ptr,SIZE_NUM);
    sem_close(semid[0]);
    sem_close(semid[1]);
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