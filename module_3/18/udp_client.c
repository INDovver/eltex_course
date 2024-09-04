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
  int my_sock;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buff[BUF_SIZE];

  printf("TCP DEMO CLIENT\n");

  // Шаг 1 - создание сокета
  my_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (my_sock < 0)
    error("ERROR opening socket");

  // Шаг 2 - получение хоста
  server = gethostbyname("127.0.0.1");
  if (server == NULL)
    error("ERROR, no such host\n");

  // Шаг 3 - заполнение структуры сервера
  memset(&serv_addr, 0, SIZE);
  serv_addr.sin_family = AF_INET;
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
  serv_addr.sin_port = htons(PORT);

  // Шаг 4 - подключение к серверу
  if (connect(my_sock, (struct sockaddr *)&serv_addr, SIZE) < 0)
    error("ERROR connecting");

  // Шаг 5 - общение с сервером
  while (1) {
    printf("YOU: ");
    fgets(buff, BUF_SIZE, stdin);
    write(my_sock, buff, strlen(buff));

    if (strcmp(buff, "quit\n") == 0) {
      printf("Exiting...\n");
      break;
    }

    int n = read(my_sock, buff, BUF_SIZE - 1);
    if (n < 0)
      error("ERROR reading from socket");
    buff[n] = 0; // Завершаем строку
    printf("SERVER: %s", buff);
  }

  close(my_sock);
  return 0;
}
