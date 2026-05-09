#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "abstractsocks.h"

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
