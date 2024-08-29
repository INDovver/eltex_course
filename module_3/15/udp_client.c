#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 1024
#define SIZE sizeof(struct sockaddr_in)

void error(const char *msg) {
  perror(msg);
  exit(0);
}

int main() {
  int my_sock, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buff[BUF_SIZE];

  printf("TCP NOT DEMO CLIENT\n");

  // Шаг 1 - создание сокета
  my_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (my_sock < 0)
    error("ERROR opening socket");

  // извлечение хоста
  server = gethostbyname(inet_ntoa((struct in_addr){
      .s_addr = htonl(
          INADDR_LOOPBACK)})); //чтобы получить строковой IP (для
                               // gethostbyname), нехобходимо макрос привести к
                               //сетевому типу, т.к. по базе он имеет интовый
                               //хостовой вид, потом привести к строковому
                               //хостовому типу, вроде нет функцию, позволяющих
                               //напрямую получить хостовую строку из интовой
                               //строковой
  if (server == NULL)
    error("ERROR, no such host\n");

  // заполнение структуры serv_addr
  memset(&serv_addr, 0, SIZE);
  serv_addr.sin_family = AF_INET;
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
  serv_addr.sin_port = htons(PORT);

  // Шаг 2 - установка соединения
  if (connect(my_sock, (struct sockaddr *)&serv_addr, SIZE) < 0)
    error("ERROR connecting");

  // Шаг 3 - чтение и передача сообщений
  int i = 0;
  while ((n = read(my_sock, buff, BUF_SIZE)) > 0) {
    buff[n] = 0;
    printf("RECEIVE:%s", buff);
    if (i % 3 == 0 && i != 0) {
      n = read(my_sock, buff, BUF_SIZE);
      buff[n] = 0;
      printf("RECEIVE:%s", buff);
      i = 0;
    }

    printf("SND:");
    fgets(buff, BUF_SIZE, stdin);
    write(my_sock, buff, strlen(buff));

    if (!strcmp(buff, "quit\n")) {
      printf("Exit...\n");
      close(my_sock);
      return 0;
    }
    i++;
  }

  printf("Recv error \n");
  close(my_sock);
  return -1;
}