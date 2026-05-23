#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include "abstractsocks.h"

int main(){
  int socketFD = createTCPIPv4Socket();

  const char* ip = "127.0.0.1";
  const int port = 5868;

  struct sockaddr_in address = createIPv4Address(ip, port);

  int result = connect(socketFD, (struct sockaddr*) &address, sizeof(address));
  if (result == 0){
    printf("Socket Connection Success!\n");
  }
  else{ 
    perror("Socket Connection Oopsies");
    exit(1);
  }
  char* line = NULL;
  size_t line_size = 0;
  printf("Type and share or die, exit to leave:\n");
  while(true){
    ssize_t charCount = getline(&line, &line_size, stdin);
    if (charCount <= -1){
      perror("Line fetching Oopsies");
    }
    if (strcmp(line, "exit\n")==0){
      printf("Exiting NOW cuz SOMEONE quit!\n");
      break;
    }
    ssize_t sent = send(socketFD, line, charCount,0);
    if (sent <= -1){
      perror("Transmission Failed");
    }
    printf("%zd bytes sent\n", sent);
  }
  // char buffer[10000];
  // recv(socketFD, buffer, 10000, 0);
  // printf("%s\n", buffer);
  
  close(socketFD);
  return 0;
}
