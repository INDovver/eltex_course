#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

#define MSG_SIZE 100
#define TERMINATE_MSG_TYPE 999
#define QUEUE_NAME "/queueue"

typedef struct msg_buffer {
    unsigned msg_type;
    char msg_text[MSG_SIZE];
}msg_buffer;

int main() {
    mqd_t msgid;
    struct mq_attr attr;
    msg_buffer message;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;
    attr.mq_maxmsg = 10;
    attr.mq_flags = 0;
    
    if((msgid = mq_open(QUEUE_NAME, O_RDWR|O_CREAT,0666,&attr))==(mqd_t)-1){
        perror("mq error");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Send: ");
        scanf("%u",&message.msg_type);
        while(getchar()!=' ');
        fgets(message.msg_text,sizeof(message.msg_text),stdin);
        message.msg_text[strcspn(message.msg_text,"\n")]='\0';

        if(mq_send(msgid, message.msg_text, sizeof(message.msg_text), message.msg_type)==-1){
            perror("send error");
            mq_close(msgid);
            exit(EXIT_FAILURE);
        }

        if(message.msg_type==TERMINATE_MSG_TYPE){
            printf("Exiting\n");
            mq_unlink(QUEUE_NAME);
            mq_close(msgid);
            exit(EXIT_SUCCESS);
        }

        printf("Receive: ");
        fflush(stdout);
        if((mq_receive(msgid, message.msg_text, sizeof(message.msg_text), &message.msg_type))==-1){
            perror("send error");
            mq_close(msgid);
            exit(EXIT_FAILURE);
        }

        if(message.msg_type==TERMINATE_MSG_TYPE){
            printf("Exiting\n");
            mq_close(msgid);
            exit(EXIT_SUCCESS);
        }

        printf("%s\n", message.msg_text);
    }
}



