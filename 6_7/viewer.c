#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Неверные аргументы: %s <ip> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in observer_addr;
  memset(&observer_addr, 0, sizeof(observer_addr));
  observer_addr.sin_family = AF_INET;
  observer_addr.sin_port = htons(atoi(argv[2]));
  if (inet_pton(AF_INET, argv[1], &observer_addr.sin_addr.s_addr) <= 0) {
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }

  if (bind(sockfd, (struct sockaddr *)&observer_addr, sizeof(observer_addr)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

	printf("Наблюдатель подключился к серверу.\n");

  while (1) {
    char buffer[256] = {0};
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_size = sizeof(sender_addr);
    int recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&sender_addr, &sender_addr_size);
    if (recv_len == -1) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }
  }

  close(sockfd);
  return 0;
}
