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
  int max_sd, activity, i;
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen = sizeof(cli_addr);
  fd_set readfds;
  fd_set prime;
  FD_ZERO(&readfds);
  FD_ZERO(&prime);
  char buffer[BUF_SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("ERROR opening socket");
  }
  FD_SET(sockfd, &prime);
  max_sd = sockfd;

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
    readfds = prime;

    // Ожидание активности на одном из сокетов
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
    for (i = 0; i <= max_sd; i++) {
      if (FD_ISSET(i, &readfds)) {
        // Обработка нового подключения
        if (i == sockfd) {
          newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
          if (newsockfd < 0) {
            error("ERROR on accept");
          }
          printf("New client\n");
          if (newsockfd > max_sd)
            max_sd = newsockfd;
          FD_SET(newsockfd, &prime);
        } else {
          strcpy(buffer, "\0");
          int valread = read(i, buffer, BUF_SIZE);
          if (valread == 0) {
            close(i);
            FD_CLR(i, &prime);
          } else {
            buffer[valread] = '\0';
            printf("Received: %s", buffer);
            int a, b;
            char sign;
            if (sscanf(buffer, "%d %c %d", &a, &sign, &b) != 3) {
              write(i, "Wrong record\n", strlen("Wrong record\n"));
              continue;
            }
            int result = myfunc(a, b, sign);
            snprintf(buffer, BUF_SIZE, "Result: %d\n", result);
            write(i, buffer, strlen(buffer));
          }
        }
      }
    }
  }

  close(sockfd);
  return 0;
}
