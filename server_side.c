#include <stdio.h>
#include "abstractsocks.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void* client_threadey(void* args){
  int clientSocketFD = *(int *)args;
  free (args);
  char buffer[1024];
  while(1){
    ssize_t bytesRecieved = recv(clientSocketFD, buffer, 1023, 0);
    if(bytesRecieved == 0){
      printf("Okay client said ba baiii\n");
      break;
    }
    if(bytesRecieved <0){
      perror("Client Connection Woopsies");
      continue;
    } 
    if(buffer[bytesRecieved-1]=='\n'){
      buffer[bytesRecieved]='\0';
    }
    else{
      buffer[bytesRecieved]='\n';
      buffer[bytesRecieved + 1] = '\0';
    }
    printf("Response was %s", buffer);
  }
  close(clientSocketFD);
  return NULL;
}
int main(){
  int serverSocketFD = createTCPIPv4Socket();
  // empty string basically means listen to requests from any ip
  struct sockaddr_in serverAddress = createIPv4Address("", 5868);
  int bindresult = bind(serverSocketFD, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
  if(bindresult == 0){
    printf("Socket was bound succesfully\n");
  }
  else{
    perror("Socket Binding Oopsies");
    exit(1);
  }
  int lsnresult = listen(serverSocketFD, 10);
  if (lsnresult == 0){
    printf("Socket listening successfully\n");
  }
  else {
    perror("Socket Listening Oopsies");
    exit(1);
  }
  while(1){
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof (struct sockaddr_in);
    int* clientSocketFD = malloc(sizeof(int));
    *clientSocketFD = accept(serverSocketFD, (struct sockaddr*) &clientAddress, &clientAddressSize);
    if(*clientSocketFD < 0){
      perror("Socket Accepting Oopsies");
      free(clientSocketFD);
      continue;
    }
    pthread_t thread_id;
    if(pthread_create(&thread_id, NULL, client_threadey, clientSocketFD)!=0){
      perror("Thread Oopsies");
      close(*clientSocketFD);
      free(clientSocketFD);
      continue;
    }
    pthread_detach(thread_id);
  }

  shutdown(serverSocketFD, SHUT_RDWR);
  close(serverSocketFD);
  return 0;
}
