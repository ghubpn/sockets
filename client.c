#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
int main( int args, char *argv[] ) {
// create socket
int clisock;
clisock = socket( AF_INET, SOCK_STREAM, 0 );

// create client socket address
struct sockaddr_in servaddr;
memset( &servaddr, 0, sizeof(servaddr) );
servaddr.sin_family = AF_INET;
servaddr.sin_port = htons(8888);
servaddr.sin_addr.s_addr = inet_addr("172.17.48.51");
//printf("yomamma\n");

// connect to server once accepted
if (connect( clisock, (struct sockaddr *)&servaddr, sizeof(servaddr) ) < 0){
  printf("connection failed\n");
  return 1;
}

printf("connected\n");

//keep communication with server 
while(1){

  printf("Enter commands\n");
  char buf[6000];
  scanf("%s", buf);
  printf("\n");
  ssize_t bytesSent = send( clisock, buf, strlen(buf), 0 );
  memset(buf, 0, sizeof(buf));
  if(bytesSent < 0){
    printf("send failed");
    return 1;
  }
  printf("sent message of size %d\n", bytesSent);

  char buf2[6000];

  if(recv(clisock, buf2, 6000, 0) < 0){
    printf("receive failed\n");
    return 1;
  }
  printf("server reply\n");
  printf("%s\n", buf2);
  memset(buf2, 0, sizeof(buf2));

}

// close socket, once finished
close( clisock );

return 0;
}
