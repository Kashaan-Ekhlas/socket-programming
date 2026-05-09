#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int createTCPIPv4Socket(){
  int socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFD == -1){
    perror("Socket Creation Oopsies");
    exit(1);
  }
  return socketFD;
}

struct sockaddr_in createIPv4Address(const char* ip, const int port){
  struct sockaddr_in address = { 
    .sin_family = AF_INET, 
    .sin_port = htons(port) 
  };
  if (inet_pton(AF_INET, ip, &address.sin_addr) <= 0){
    perror("Inet PTON Oopsies");
    exit(1);
  }
  return address;
}

int main(){
  int socketFD = createTCPIPv4Socket();

  const char* ip = "142.250.188.46";
  const int port = 80;

  struct sockaddr_in address = createIPv4Address(ip, port);

  int result = connect(socketFD, (struct sockaddr*) &address, sizeof(address));
  if (result == 0){
    printf("Socket Connection Success!\n");
  }
  else{ 
    perror("Socket Connection Oopsies");
    exit(1);
  }
  
  char* msg_getreq = "GET / HTTP/1.1\r\nHost: google.com\r\nConnection: close\r\n\r\n";
  ssize_t sent = send(socketFD, msg_getreq, strlen(msg_getreq), 0);
  if (sent == -1){
    perror("Transmission Failed");
    exit(1);
  }
  printf("%zd bytes sent\n", sent);
  char buffer[10000];
  recv(socketFD, buffer, 10000, 0);
  printf("%s\n", buffer);

  close(socketFD);
  return 0;
}
