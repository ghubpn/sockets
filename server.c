#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

int main( int args, char *argv[] ) {

// create socket
int servsock = socket( AF_INET, SOCK_STREAM, 0 );
if(servsock == -1){
	printf("socket could not be created\n");
	return 1;
}

printf("socket created\n");

// create server socket address
struct sockaddr_in servaddr;
memset( &servaddr, 0, sizeof(servaddr) );
servaddr.sin_family = AF_INET;
servaddr.sin_port = htons(5432);
servaddr.sin_addr.s_addr = INADDR_ANY;

// configure servsock using servaddr
if(bind( servsock, (struct sockaddr *)&servaddr, sizeof(servaddr) ) < 0){
	printf("bind failed\n");
	return 1;
}

printf("bind completed\n");

// make servsock listen for incoming client connections
listen( servsock, 5 );
printf("listenning....\n");

// accept client connection and log connection information
struct sockaddr_in cliaddr;
int cliLen;
cliLen = sizeof( struct sockaddr_in );
int clisock = accept( servsock, (struct sockaddr *)&cliaddr, (socklen_t*) &cliLen );
if(clisock < 0){
	printf("Accept failed\n");
	return 1;
}
printf("connection accepted\n");

char buf[2000];
int bytesRecv;

while( bytesRecv = recv( clisock, buf, 2000, 0 ) > 0) {

  // 1 - LIST CONTENTS OF CURRENT DIRECTORY *************************************
  if(buf[0] == '1'){
    char listBuf[2000]; //array to hold the list of the directory
    memset(buf, 0, sizeof(buf)); //resets the buf array
    char *ptr = NULL; // to point to the current working directory
    DIR *dPtr = NULL; // directory pointer to the path of the directory
    struct dirent *strDir = NULL; //struct to read the contents of the directory
    char test[200]; // to hold current working directory
    getcwd(test, 200); //to get the current working directory
    ptr = test; //to point ptr to the current working directory
    dPtr = opendir((const char*) ptr); //to open the directory and point the DIR variable to the directory
    //while loop to read the directory contents
    while((strDir = readdir(dPtr)) != NULL){
       strcpy(buf, strDir->d_name); //copy contents of directory in buf
       strcat(buf, "  \t"); //to add a tab to the end of a content in the directory
       strcat(listBuf, buf); //to concat the newly read content to the previously read content stored in listBuf
       memset(buf, 0, sizeof(buf)); // to reset buf to ensure no duplication
    }
    strcpy(buf, listBuf);
    memset(listBuf, 0, sizeof(listBuf));
    send(clisock, buf, 2000, 0);
    memset(buf, 0, sizeof(buf)); // to reset the buf
  }
  
  // 2 - GET A FILE FROM SERVER *************************************************
  else if( buf[0] == '2') {
    FILE* file;
    char  filename[200];
    memset(buf, 0, sizeof(buf));
    recv( clisock, filename, 2000, 0 );
    //FILE* file;
    //char fName[200];
    //fName[0] = '.';
    //fName[1] = '/'; //to look in current directory
    //strcat(fName, buf); //concat the current directory symbol to the file name
    file = fopen(filename, "r"); //open the file name for reading
    printf("file name is %s\n", filename);
    memset(filename, 0, sizeof(filename));
    if(file == NULL){
      printf("file did not open\n");
      return 1;
    }
    memset(buf, 0, sizeof(buf));
    fscanf(file, "%s", buf);
    //printf("%s\n", buf);
    send(clisock, buf, 2000, 0);
    //printf("got here!!\n");
    memset(buf, 0, sizeof(buf)); // to reset the buf
    fclose(file); //close the file
    printf("closed file!\n");
  }

  // 3 - SEND A FILE TO SERVER ****************************************************
  else if( buf[0] == '3') {
    memset(buf, 0, sizeof(buf));
    strcpy(buf, "_PUT_COMMAND_\n");
    send(clisock, buf, 2000, 0);
    memset(buf, 0, sizeof(buf)); // to reset the buf
  }
  
  // 4 - CHANGE DIRECTORY *********************************************************
  else if (buf[0] == '4'){
    memset(buf, 0, sizeof(buf));
    bytesRecv = recv(clisock, buf, 2000, 0); //to receive directory name
    int status; //check to ensure changing the dir was successful
    status = chdir(buf); //change the directory
    if(status == 0){
      printf("changes dir");
    }
    strcpy(buf, "_DIR_CHANGED_\n");
    send(clisock, buf, 2000, 0);
    memset(buf, 0, sizeof(buf)); // to reset the buf
  }

  // 5 - CREATE DIRECTORY *********************************************************
  else if (buf[0] == '5'){
    memset(buf, 0, sizeof(buf)); //reset of buf
    bytesRecv = recv(clisock, buf, 2000, 0); //to receive directory name
    int status;
    status = mkdir(buf, S_IRWXU | S_IRWXG | S_IRWXO); //make dir with name in buf and give permissions
    memset(buf, 0, sizeof(buf)); //reset buf
    strcpy(buf, "Directory Made"); //to provide feedback to client
    send(clisock, buf, 2000, 0);
    memset(buf, 0, sizeof(buf)); // to reset the buf
  }
}

// close socket, once finished
close( servsock );
printf("socket closed\n");
return 0;
}
