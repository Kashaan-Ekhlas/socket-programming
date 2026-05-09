#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
int main(){
  int socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFD == -1){
    perror("Socket Creation Oopsies");
    return 1;
  }

  char* ip = "142.250.188.46";
  int port = 80;

  struct sockaddr_in address = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
  };
  inet_pton(AF_INET, ip, &address.sin_addr);

  int result = connect(socketFD, (struct sockaddr*) &address, sizeof(address));
  if (result == 0){
    printf("Socket Connection Success!\n");
  }
  else{ 
    perror("Socket Connection Oopsies");
  }
  
  char* buffer = "meow_maww";
  ssize_t sent = send(socketFD, buffer, strlen(buffer), 0);
  if (sent == -1){
    perror("Transmission Failed");
  }
  close(socketFD);
  return 0;
}
