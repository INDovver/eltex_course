#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 1024
#define SIZE sizeof(struct sockaddr_in)
#define str1 "Enter 1 parameter or enter \"get [filename.format]\"\n"
#define str2 "Enter action (+-/*)\n"
#define str3 "Enter 3 parameter\n"
#define str4 "Wrong action\n"
#define str5 "Cant devide by zero\n"

// дескрипторы сокетов
int sockfd, newsockfd;

// количество активных пользователей
int nclients = 0;

//дочерний отправляет родителю сигнал уменьшения кол-ва клиентов
void clientsdecrease(int signum) { nclients--; }

// чтобы можно было закрывать сервер через ctrl+c
void sign(int signum) {
  printf("\n");
  close(sockfd);
  exit(EXIT_SUCCESS);
}

// печать количества активных пользователей
void printusers(int signum) {
  if (nclients) {
    printf("%d user on-line\n", nclients);
  } else {
    printf("No User on line\n");
  }
}

// функция обработки ошибок
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
    return a / b;
  }
}

int quit_check(char *buff) {
  if (strcmp(buff, "quit\n") == 0) {
    printf("Client requested to quit.\n");
    return 1;
  } else
    return 0;
}

int write_read_func(char *buff, const char *str, int sock) {
  int bytes_recv; // размер принятого сообщения
  write(sock, str, strlen(str));
  bytes_recv = read(sock, buff, BUF_SIZE);
  if (bytes_recv < 0)
    error("ERROR reading from socket\n");
  buff[bytes_recv] = 0;
  return quit_check(buff);
}

// функция обслуживания подключившихся пользователей
void dostuff(int sock) {
  int bytes_recv = 0, bytes_send = 0;
  int a, b;  // переменные для myfunc
  char sign; //для определения действия
  char buff[BUF_SIZE] = {};
  char file_name[BUF_SIZE] = {};
  FILE *fp = 0;
  do {
    if (write_read_func(buff, str1, sock))
      break;
    if (!strncmp(buff, "get ", 4)) {
      sscanf(buff + 4, "%s", file_name);
      if ((fp = fopen(file_name, "r")) == 0) {
        write(sock, "file doesn't exist\n", strlen("file doesn't exist\n"));
        usleep(150);
        continue;
      }
      while ((bytes_recv = fread(buff, 1, BUF_SIZE, fp)) > 0) {
        bytes_send = write(sock, buff, bytes_recv);
        while (bytes_send != bytes_recv) {
          bytes_send += write(sock, buff + bytes_send, bytes_recv - bytes_send);
        }
      }
      fclose(fp);
      sleep(1); //чтобы EOF корректно отправился в одном пакете, а не разделился
      write(sock, "EOF", 3);
      sleep(1);
      continue;
    }
    a = atoi(buff); // преобразование первого параметра в int
    if (write_read_func(buff, str2, sock))
      break;
    sign = buff[0]; // получение действия
    if (write_read_func(buff, str3, sock))
      break;
    b = atoi(buff); // преобразование второго параметра в int
    if (!strchr("+-*/", sign)) {
      write(sock, str4, strlen(str4));
      usleep(150);
    } else if (sign == '/' && b == 0) {
      write(sock, str5, strlen(str5));
      usleep(150); //ограничивает два идущих подряд write
    } else {
      a = myfunc(a, b, sign);
      snprintf(buff, BUF_SIZE - 1, "result is %d",
               a); // преобразование результата в строку
      buff[strlen(buff)] = '\n';
      write(sock, buff, strlen(buff));
      usleep(150); //ограничивает два идущих подряд write
    }
  } while (1); // Бесконечный цикл до получения команды "quit"

  nclients--; // уменьшаем счетчик активных клиентов
  kill(getppid(), SIGUSR1);
  usleep(150); //чтобы один сигнал не прерывал другой
  kill(getppid(), SIGUSR2);
  close(sock);
  return;
}

int main() {
  int pid;          // id номер потока
  socklen_t clilen; // размер адреса клиента типа socklen_t
  struct hostent *hst;
  struct sockaddr_in serv_addr,
      cli_addr; // структура сокета сервера и клиента
  printf("TCP SERVER NOT DEMO\n");

  signal(SIGUSR2, printusers);
  signal(SIGINT, sign);
  signal(SIGUSR1, clientsdecrease);

  // Шаг 1 - создание сокета
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  // Шаг 2 - связывание сокета с локальным адресом
  memset(&serv_addr, 0, SIZE);
  memset(&cli_addr, 0, SIZE);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  serv_addr.sin_port = htons(PORT);
  if (bind(sockfd, (struct sockaddr *)&serv_addr, SIZE) < 0)
    error("ERROR on binding");

  // Шаг 3 - ожидание подключений, размер очереди - 5
  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  // Шаг 4 - извлекаем сообщение из очереди (цикл извлечения запросов на
  // подключение)
  while (1) {
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
      error("ERROR on accept");
    nclients++;

    // вывод сведений о клиенте
    hst = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
    printf("+%s [%s] new connect!\n", (hst) ? hst->h_name : "Unknown host",
           (char *)inet_ntoa(cli_addr.sin_addr));
    raise(SIGUSR2);
    pid = fork();
    if (pid < 0)
      error("ERROR on fork");
    if (pid == 0) {
      close(sockfd);
      dostuff(newsockfd);
      exit(0);
    } else
      close(newsockfd);
  }
  close(sockfd);
  return 0;
}
