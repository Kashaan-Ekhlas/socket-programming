#include <stdio.h>
#include "abstractsocks.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define MAX_CLIENTS 169

int clientFDs[MAX_CLIENTS];
int clientCount = 0;
pthread_mutex_t clientListMutex = PTHREAD_MUTEX_INITIALIZER;

int addClient(int clientSocketFD){
  pthread_mutex_lock(&clientListMutex);
  if(clientCount == MAX_CLIENTS){
    pthread_mutex_unlock(&clientListMutex);
    return 1;
  }
  clientFDs[clientCount++] = clientSocketFD;
  pthread_mutex_unlock(&clientListMutex);
  return 0;
}

void removeClient(int clientSocketFD){
  pthread_mutex_lock(&clientListMutex);
  for(int i = 0; i < clientCount; i++){
    if(clientFDs[i] == clientSocketFD){
      for (int j = i; j < clientCount - 1; j++) {
        clientFDs[j] = clientFDs[j + 1];
      }
      clientCount--;
      break;
    }
  }
  pthread_mutex_unlock(&clientListMutex);
  return;
}
int broadcastMsg(int senderFD, char buffer[], ssize_t bytesToSend){

  pthread_mutex_lock(&clientListMutex);
  int clientCountCopy = clientCount;
  if (clientCountCopy == 0){
    pthread_mutex_unlock(&clientListMutex);
    return 0;
  }
  int clientFDsCopy[clientCountCopy];
  for(int i = 0; i < clientCountCopy; i++){
    clientFDsCopy[i] = clientFDs[i];
  }
  pthread_mutex_unlock(&clientListMutex);

  int failed_count = 0;
  for(int i = 0; i < clientCountCopy; i++){
    if(clientFDsCopy[i] != senderFD){
      ssize_t sent = send(clientFDsCopy[i], buffer, bytesToSend, 0);
      if (sent <= -1){
        failed_count++;
      }
    }
  }
  return failed_count;
}


void* client_threadey(void* args){
  int clientSocketFD = *(int* )args;
  free (args);

  if(addClient(clientSocketFD)){
    printf("Overflow, cannot add client\n");
    close(clientSocketFD);
    return NULL;
  }

  unsigned char buffer[1024];
  while(1){
    ssize_t bytesRecieved = recv(clientSocketFD, buffer, 1022, 0);
    if(bytesRecieved == 0){
      printf("Okay client said ba baiii\n");
      break;
    }
    if(bytesRecieved < 0){
      perror("Client Connection Woopsies");
      continue;
    } 
    for(int i = 0; i < bytesRecieved; i++){
      if(buffer[i] < 32 && buffer[i]!='\n'){
        buffer[i] = '?';
      }
    }
    if(buffer[bytesRecieved-1]=='\n'){
      buffer[bytesRecieved]='\0';
    }
    else{
      buffer[bytesRecieved]='\n';
      buffer[bytesRecieved + 1] = '\0';
    }

    printf("User %d said: %s", clientSocketFD, buffer);

    int failed_count = broadcastMsg(clientSocketFD, buffer, bytesRecieved+1);

    if(failed_count){
      printf("Transmission Failed for: %d users\n", failed_count);
    }
  }

  removeClient(clientSocketFD);

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
