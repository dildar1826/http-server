#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
int main() { 
  char buffer[1024] = {0};
  
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
    printf("Failed to open a scoekt...");
  struct sockaddr_in address = {0};

  address.sin_family = AF_INET;
  address.sin_port = htons(5631);
  address.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (const struct sockaddr *)&address, sizeof(address)) == -1) {
    perror("Failed to bind socket.....");
    close(sockfd);
    exit(1);
  }
 
  printf("Socket successfully bound to port.");

  if (listen(sockfd, 5) < 0) {
    perror("Failed to initiate listen..,");
    close(sockfd);
    exit(1);
  }

  printf("Listening started successfully......");

  struct sockaddr_in client_addr = {0};

  while (1) {
    socklen_t addr_len = sizeof(client_addr);
    int clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

    if (clientfd < 0) {
      perror("Failed to accept connection..");
      close(sockfd);
      continue;
    }

    ssize_t bytes_received = recv(clientfd, buffer, sizeof(buffer) -1, 0);
    if (bytes_received < 0){
      perror("Receive Failed.");
    }
    else if (bytes_received == 0) {
      printf("Client Disconnected.");
    }
    else {
      printf("Received %zd bytes: \n %s\n", bytes_received, buffer);

      const char response [] = 
          "HTTP/1.1 200 OK\r\n"
          "Content-Type: text/plain\r\n"
          "Content-Length: 13 \r\n"
          "\r\n"
          "Hello, world!";
      ssize_t sent = send(clientfd, response, sizeof(response) - 1, 0);

      if(sent < 0){
      perror("Send failed");
      }
      
    }
    close(clientfd);
  }
  close(sockfd);
  return 0;
}