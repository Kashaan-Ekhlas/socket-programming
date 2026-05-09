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
  
  char* msg2_getreq = "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n";
  ssize_t sent2 = send(socketFD, msg2_getreq, strlen(msg2_getreq), 0);
  if (sent2 == -1){
    perror("Transmission Failed");
  }
  printf("%zd bytes sent\n", sent2);
  char buffer2[10000];
  recv(socketFD, buffer2, 10000, 0);
  printf("%s\n", buffer2);

  close(socketFD);
  return 0;
}
