#include <stdio.h>
#include "abstractsocks.h"
#include <stdlib.h>

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
  
  struct sockaddr_in clientAddress;
  int clientAddressSize = sizeof (struct sockaddr_in);
  int clientSocketFD = accept(serverSocketFD, (struct sockaddr*) &clientAddress, &clientAddressSize);
  if(clientSocketFD < 0){
    perror("Socket Accepting Woopsies");
  }

  char buffer[1024];
  recv(clientSocketFD, buffer, 1024, 0);

  printf("Response was %s\n", buffer);
  return 0;
}
