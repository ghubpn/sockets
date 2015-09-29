#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>

int main( int args, char *argv[] ) {

  // create socket
  int clisock;
  clisock = socket( AF_INET, SOCK_STREAM, 0 );

  // create client socket address
  struct sockaddr_in servaddr;
  memset( &servaddr, 0, sizeof(servaddr) );
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(5432);
  servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");

  // connect to server once accepted
  if (connect( clisock, (struct sockaddr *)&servaddr, sizeof(servaddr) ) < 0){
    printf("connection failed\n");
    exit(1);
  }
  printf("connected\n");

  // variables
  char    buf[1];
  char    name[200];
  char    data[6000];
  FILE    *file;
 
  // keep communication with server 
  while(1){

    // clear all buffers before each iteration
    memset(buf,  0, sizeof(buf));
    memset(data, 0, sizeof(data));
    memset(name, 0, sizeof(name));

    //print menu
    printf("Please pick an option from the menu\n");
    printf("-----------------------------------\n");
    printf("[1] --- List the contents of the current directory\n"); //implements ls
    printf("[2] --- Get a file from the server\n"); // implements get file-name
    printf("[3] --- Send a file to the server\n"); //implements put file-name
    printf("[4] --- Change Directory\n"); //implements cd
    printf("[5] --- Make a new directory\n"); //implements mkdir directory-name
    printf("Selection: ");

    // get user's selection and send it to server
    scanf("%s", buf);
    send( clisock, buf, strlen(buf), 0 );

    // 1 - LIST CONTENTS OF CURRENT DIRECTORY *************************************
    if(buf[0] == '1'){
      memset(buf, 0, sizeof(buf));
      printf("Listing...\n");
      if(recv(clisock, data, 6000, 0) < 0){
	printf("receive failed\n");
	exit(1);
      }
      printf("server reply:\n\n");
      printf("%s\n\n", data);
    }

    // 2 - GET A FILE FROM SERVER *************************************************
    else if(buf[0] == '2'){
      memset(buf, 0, sizeof(buf));
      printf("Please Enter File Name To Receive\n");
      scanf("%s", name);
      file = fopen(name, "wb");
      if (file == NULL) {
	printf("Could not open file.\n");
	exit(1);
      }
      send( clisock, name, strlen(name), 0 );
      memset(name, 0, sizeof(name));
      if(recv(clisock, buf, sizeof(char), 0) < 0){
	printf("receive failed: size\n");
	exit(1);
      }
      printf("%d\n", (int)buf[0]);
      if(recv(clisock, data, (int)buf[0], 0) < 0){
	printf("receive failed\n");
	exit(1);
      }
      memset(buf, 0, sizeof(buf));
      //fprintf(file, "%s", data);
      fwrite(data, 1, (int)buf[0], file);
      memset(data, 0, sizeof(data));
      fclose(file);
    }

    // 3 - SEND A FILE TO SERVER ****************************************************
    else if(buf[0] == '3'){
      memset(buf, 0, sizeof(buf));
      printf("Please Enter File Name To Send\n");
      scanf("%s", name);
      file = fopen(name, "rb");
      if (file == NULL) {
	printf("Could not open file.\n");
	exit(1);
      }
      send( clisock, name, strlen(name), 0 ); // send file name
      //fscanf(file, "%s", data); // write file into buf
      int s = sizeof(file);
      fread(data, 1, s, file);
      send( clisock, data, strlen(data), 0 ); // send file contents
      memset(data, 0, sizeof(data));
      fclose(file); // close local file
      printf("file \"%s\" sent to server\n\n", name);
      memset(name, 0, sizeof(name));
    }

    // 4 - CHANGE DIRECTORY *********************************************************
    else if(buf[0] == '4'){
      memset(buf, 0, sizeof(buf));
      printf("Please Enter Directory Name\n");
      scanf("%s", name);
      send(clisock, name, strlen(name), 0);
      if(recv(clisock, data, 6000, 0) < 0){
	printf("receive failed\n");
	exit(1);
      }
      printf("server reply:\n\n");
      printf("%s\n\n", data);
    }

    // 5 - CREATE DIRECTORY *********************************************************
    else if(buf[0] == '5'){
      memset(buf, 0, sizeof(buf));
      printf("Please Enter new Directory Name\n");
      scanf("%s", name);
      send(clisock, name, strlen(name), 0);
      if(recv(clisock, data, 6000, 0) < 0){
	printf("receive failed\n");
	exit(1);
      }
      printf("server reply:\n\n");
      printf("%s\n\n", data);
    }

    else{
      printf("Not a corrent entry, please try again\n");
    }
  }
  
  // close socket, once finished
  close( clisock );
  return 0;
} // end main
