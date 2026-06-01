#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <pthread.h>
#include "abstractsocks.h"

void* recvthreadey(void* args){
  int socketFD = *(int *)args;
  free (args);
  unsigned char buffer[1024];

  while(1){
    ssize_t bytesRecieved = recv(socketFD, buffer, 1024, 0);
    if(bytesRecieved == 0){
      printf("Server Closed Connection\n");
      break;
    }
    if(bytesRecieved < 0){
      perror("Broadcasting Oopsies");
      continue;
    } 
    printf("Anonymous User Said : %s", buffer);
  }
  close(socketFD);
  return NULL;
}

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

  int* socketFD_for_thread = malloc(sizeof(int));
  *socketFD_for_thread = socketFD;
  pthread_t recvthreadey_id;
  if(pthread_create(&recvthreadey_id, NULL, recvthreadey, socketFD_for_thread)!=0){
    perror("Thread Oopsies");
    free(socketFD_for_thread);
    close(socketFD);
    exit(1);
  } 
  pthread_detach(recvthreadey_id);

  char* line = NULL;
  size_t line_size = 0;
  printf("Type and share or die, exit to leave:\n");
  while(1){
    ssize_t charCount = getline(&line, &line_size, stdin);
    if (charCount <= -1){
      perror("Line fetching Oopsies");
      continue;
    }
    if (strcmp(line, "exit\n")==0){
      printf("Exiting NOW cuz SOMEONE quit!\n");
      break;
    }
    ssize_t sent = send(socketFD, line, charCount,0);
    if (sent <= -1){
      perror("Transmission Failed");
      break;
    }
    printf("%zd bytes sent\n", sent);
  }

  close(socketFD);
  return 0;
}
