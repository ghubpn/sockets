#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUF  64
// http://www.thegeekstuff.com/2011/12/c-socket-programming/


int serverSocket, clientSocket;

int main()
{
  char ip[MAX_BIF];
  int port, b, addrSize, bytesRecv;
  // need to assign ip and port #
  struct sockadd_in, servAddr, clientAddr; 
  
  // setup a connection
  serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
  if(serverSocket < 0){ 
    printf("couldn't open socket\n");
    exit(-1);
  }
  
  //server address
  memeset(&myAddr, 0, sizeof(servAddr)); 
  myAddr.sin_family = AF_INET;
  
  // client listens on any available address 
  myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myAddr.sin_port = htons((unsigned short) port); 
  
  // binds serverSocket
  b = bind(serverSocket, (struct sockaddr *) &myAddr, sizeof(myAddr));
  if (b < 0) { 
    printf("couldn't bind\n"); 
    exit(-1);
  }
  
  //listens
  b = listen(serverSocket, 5); 
  if (b < 0){
    printf("couldn't listen\n");
    exit(-1);
  }  
  
  //waiting for connection request
  addrSize = sizeof(clientAddr);
  
  clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addrSize); 
  if(clientSocket < 0) {
    printf("couldn't connect\n"); 
    exit(-1);
  }
  
  // receive file 
  while(1){
    bytesRecv = recv(clientSocket, buffer, sizeof(buffer), 0);
    buffer[bytesRecv] = 0;
  
  }
  
}