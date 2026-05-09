#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main(){
  int socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFD == -1){
    perror("Socket Creation Oopsies");	
  }

  char* ip = "142.250.188.46";
  int port = 80;

  struct sockaddr_in address = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
  };
  inet_pton(AF_INET, ip, &address.sin_addr.s_addr);

  int result = connect(socketFD, &address, sizeof(address));
  if (result == 0){
    printf("Socket Connection Success!\n");
  }
  else{ 
    perror("Socket Connection Oopsies\n");
  }
  return 0;
}
