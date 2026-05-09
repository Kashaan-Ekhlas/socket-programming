#include <stdio.h>
#include <>
#include "abstractsocks.h"

int main(){
  int serverSocketFD = createTCPIPv4Socket();
  // empty string basically means listen to requests from any ip
  struct sockaddr_in serverAddress = createIPv4Address("", 2000);
  return 0;
}
