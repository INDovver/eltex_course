#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 1024
#define SIZE sizeof(struct sockaddr_in)

void error(const char *msg) {
  perror(msg);
  exit(1);
}

// функция обработки данных
int myfunc(int a, int b, char sign) {
  switch (sign) {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
    return a * b;
  case '/':
    if (b == 0) {
      printf("Error: Division by zero!\n");
      return 0;
    }
    return a / b;
  default:
    return 0;
  }
}

int main() {
  int sockfd, newsockfd;
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;
  fd_set readfds;
  int client_sockets[FD_SETSIZE]; // массив клиентских сокетов
  int max_sd, activity, i, sd;
  char buffer[BUF_SIZE];

  // Инициализация всех клиентских сокетов нулями
  for (i = 0; i < FD_SETSIZE; i++) {
    client_sockets[i] = 0;
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("ERROR opening socket");
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT);

  // Привязка сокета к адресу
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    error("ERROR on binding");
  }

  // Ожидание подключений
  listen(sockfd, 5);

  while (1) {
    // Очистка и настройка файлового дескриптора
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    max_sd = sockfd;

    // Добавление клиентских сокетов в набор дескрипторов
    for (i = 0; i < FD_SETSIZE; i++) {
      sd = client_sockets[i];
      if (sd > 0) {
        FD_SET(sd, &readfds);
      }
      if (sd > max_sd) {
        max_sd = sd;
      }
    }

    // Ожидание активности на одном из сокетов
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    // Обработка нового подключения
    if (FD_ISSET(sockfd, &readfds)) {
      clilen = sizeof(cli_addr);
      newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
      if (newsockfd < 0) {
        error("ERROR on accept");
      }

      printf("New connection, socket fd is %d, ip is : %s, port : %d\n",
             newsockfd, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

      // Добавление нового сокета в массив клиентских сокетов
      for (i = 0; i < FD_SETSIZE; i++) {
        if (client_sockets[i] == 0) {
          client_sockets[i] = newsockfd;
          printf("Adding to list of sockets as %d\n", i);
          break;
        }
      }
    }

    for (i = 0; i < FD_SETSIZE; i++) {
      sd = client_sockets[i];

      if (FD_ISSET(sd, &readfds)) {
        int valread = read(sd, buffer, BUF_SIZE);
        if (valread == 0) {
          close(sd);
          client_sockets[i] = 0;
        } else {
          buffer[valread] = '\0';
          printf("Received: %s", buffer);
          int a, b;
          char sign;
          sscanf(buffer, "%d %c %d", &a, &sign, &b);
          int result = myfunc(a, b, sign);

          snprintf(buffer, BUF_SIZE, "Result: %d\n", result);
          write(sd, buffer, strlen(buffer));
        }
      }
    }
  }

  close(sockfd);
  return 0;
}
