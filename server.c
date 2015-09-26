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
//printf("debuggery\n");
servaddr.sin_family = AF_INET;
//printf("1\n");
servaddr.sin_port = htons(8888);
//printf("2\n");
servaddr.sin_addr.s_addr = INADDR_ANY; // inet_addr( INADDR_ANY );
//printf("debug1\n");

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
//memset(buf, 0, sizeof(buf));
int bytesRecv;

while( bytesRecv = recv( clisock, buf, 2000, 0 ) > 0) {
  
  if(buf[0] == '1'){
    char listBuf[2000]; //array to hold the list of the directory
    //printf("in 1\n");
    memset(buf, 0, sizeof(buf)); //resets the buf array
    char *ptr = NULL; // to point to the current working directory
    DIR *dPtr = NULL; // directory pointer to the path of the directory
    struct dirent *strDir = NULL; //struct to read the contents of the directory
    char test[200]; // to hold current working directory
    getcwd(test, 200); //to get the current working directory
    //printf("in directory %s\n", test);
    //printf("passed the struct\n");
    ptr = test; //to point ptr to the current working directory
    //printf("passed ptr\n");
    dPtr = opendir((const char*) ptr); //to open the directory and point the DIR variable to the directory
    //printf("passed dPtr\n");
    //printf("Right before the while\n");
    while((strDir = readdir(dPtr)) != NULL){
      //printf("In the while of 1\n");
       strcpy(buf, strDir->d_name); //copy contents of directory in buf
       strcat(buf, "  \t"); //to add a tab to the end of a content in the directory
       strcat(listBuf, buf); //to concat the newly read content to the previously read content stored in listBuf
       memset(buf, 0, sizeof(buf)); // to reset buf to ensure no duplication
       //send(clisock, buf, 2000, 0);

    }
    //printf("%s\n", listBuf);
    send(clisock, listBuf, 2000, 0);
    memset(listBuf, 0, sizeof(listBuf));
    //strcpy(buf, "_LIST_CONTENTS_\n");
  }
    else if( buf[0] == '2') {
    memset(buf, 0, sizeof(buf));
    strcpy(buf, "_GET_COMMAND_\n");
  }
  else if( buf[0] == '3') {
    memset(buf, 0, sizeof(buf));
    strcpy(buf, "_PUT_COMMAND_\n");
  }
  else if (buf[0] == '4'){
    memset(buf, 0, sizeof(buf));
    bytesRecv = recv(clisock, buf, 2000, 0); //to receive directory name
    int status; //check to ensure changing the dir was successful
    status = chdir(buf);
    if(status == 0){
      printf("changes dir");
    }
  }
  else if (buf[0] == '5'){
    //in the mkdir function
  memset(buf, 0, sizeof(buf)); ///reset of buf
  //strcpy(buf, "IN_MKDIR_");
  bytesRecv = recv(clisock, buf, 2000, 0); //to receive directory name
  int status;
  status = mkdir(buf, ACCESSPERMS); //make dir with name in buf and give permissions
  memset(buf, 0, sizeof(buf)); //reset buf
  strcpy(buf, "Directory Made"); //to provide feedback to client
  }

  send(clisock, buf, 2000, 0);
  memset(buf, 0, sizeof(buf)); // to reset the buf
}

// close socket, once finished
close( servsock );
printf("socket closed\n");
return 0;
}
