#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 1024

void process_packet(unsigned char *buffer, int size);

int main() {
  int raw_sock;
  struct sockaddr_in source_socket_address;
  int address_len = sizeof(source_socket_address);
  unsigned char *buffer = (unsigned char *)malloc(BUF_SIZE);

  raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (raw_sock < 0) {
    perror("Socket creation error");
    return 1;
  }

  while (1) {
    int data_size =
        recvfrom(raw_sock, buffer, BUF_SIZE, 0,
                 (struct sockaddr *)&source_socket_address, &address_len);
    if (data_size < 0) {
      perror("Recvfrom error");
      return 1;
    }
    process_packet(buffer, data_size);
  }

  close(raw_sock);
  free(buffer);
  return 0;
}

void process_packet(unsigned char *buffer, int size) {
  struct iphdr *iph = (struct iphdr *)buffer;
  if (iph->protocol == 17) {
    struct udphdr *udph = (struct udphdr *)(buffer + iph->ihl * 4);

    if (ntohs(udph->dest) == PORT) {
      printf("\nReceived Packet Size: %d bytes\n", size);
      printf("Source IP: %s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
      printf("Destination IP: %s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
      printf("Source Port: %d\n", ntohs(udph->source));
      printf("Destination Port: %d\n", ntohs(udph->dest));
      printf("Data: %s\n", buffer + iph->ihl * 4 + sizeof(struct udphdr));
    }
  }
}
