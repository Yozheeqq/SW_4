#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
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

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr.s_addr) <= 0) {
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  srand(time(NULL));

  while (1) {
    int num1 = rand() % 3;
    int num2;
    do {
      num2 = rand() % 3;
    } while (num1 == num2);

    uint8_t buffer[2] = {num1, num2};
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int recv_len = recvfrom(sockfd, &client_addr, sizeof(client_addr), 0, (struct sockaddr *)&client_addr, &client_addr_size);
    printf("Медиатор выложил компоненты %d и %d\n", num1, num2);
		if (recv_len == -1) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }
    if (sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, client_addr_size) == -1) {
      perror("sendto");
      exit(EXIT_FAILURE);
    }
    sleep(1);
  }
  close(sockfd);
  return 0;
}
