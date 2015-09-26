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
servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
//printf("yomamma\n");

// connect to server once accepted
if (connect( clisock, (struct sockaddr *)&servaddr, sizeof(servaddr) ) < 0){
  printf("connection failed\n");
  return 1;
}

printf("connected\n");

//keep communication with server 
while(1){

  //added on 21/09/2015 by Adam
  printf("Please pick an option from the menu\n");
  printf("-----------------------------------\n");
  printf("[1] --- List the contents of the current directory\n"); //implements ls
  printf("[2] --- Get a file from the server\n"); // implements get file-name
  printf("[3] --- Send a file to the server\n"); //implements put file-name
  printf("[4] --- Change Directory\n"); //implements cd
  printf("[5] --- Make a new directory\n"); //implements mkdir directory-name
  printf("Selection: ");
  char buf[1];
  char dirName[200];
  scanf("%s", buf);

  ssize_t bytesSent;

  if(buf[0] == '1'){
    bytesSent = send( clisock, buf, strlen(buf), 0 );
    printf("Listing.....\n");
    
  } 
  else if(buf[0] == '2'){
    bytesSent = send( clisock, buf, strlen(buf), 0 );
    printf("Please Enter File Name To Receive\n");
  }
  else if(buf[0] == '3'){
    bytesSent = send( clisock, buf, strlen(buf), 0 );
    printf("Please Enter File Name To Send\n");
  } 
  else if(buf[0] == '4'){
    bytesSent = send( clisock, buf, strlen(buf), 0 );
    printf("Please Enter Directory Name\n");
    scanf("%s", dirName);
    bytesSent = send(clisock, dirName, strlen(dirName), 0);
  } 
  else if(buf[0] == '5'){
    bytesSent = send( clisock, buf, strlen(buf), 0 );
    printf("Please Enter new Directory Name\n");
    scanf("%s", dirName);
    bytesSent = send(clisock, dirName, strlen(dirName), 0);
  } else{
    printf("Not a corrent entry, please try again\n");
  }
  //editted up to here by Adam

  printf("\n");
   //bytesSent = send( clisock, buf, strlen(buf), 0 );
  memset(buf, 0, sizeof(buf));
  /*if(bytesSent < 0){
    printf("send failed");
    //return 1;
  }*/
  //printf("sent message of size %d\n", bytesSent);

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
