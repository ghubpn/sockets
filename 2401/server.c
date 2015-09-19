#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUF  64

int serverSocket, clientSocket; 

int main()
{
  char ip[MAX_BUF];
  int port, i, addrSize, bytesRecv; 
  struct sockaddr_in myAddr, clientAddr; 
  char buffer[30];
  
  serverSocket = socket(PF_INET, SOCK_STREAM< IPPROTO_TCP); 
  if(clientSocket < 0){
    printf("couldn't open socket\n"); 
    exit(-1)
  }

  // setup server address
  memset(&myAddr

}