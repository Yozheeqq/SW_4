#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Неверные аргументы: %s <ip> <port> <number>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int my_number = atoi(argv[3]);

  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr.s_addr) <= 0) {
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }

  while (1) {
    uint8_t buffer[2] = {0};
    if (sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
      perror("sendto");
      exit(EXIT_FAILURE);
    }
    int recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    if (recv_len == -1) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }
    int missing_number = -1;
    for (int i = 0; i < 3; ++i) {
      if (i != buffer[0] && i != buffer[1]) {
        missing_number = i;
        break;
      }
    }
    if (my_number == missing_number) {
      printf("Курильщик %d скрутил сигарету и курит\n", missing_number);
    }
    sleep(1);
  }
  close(sockfd);
  return 0;
}
