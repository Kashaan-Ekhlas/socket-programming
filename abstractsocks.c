#include "abstractsocks.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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