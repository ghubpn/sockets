#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
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
int bytesRecv;

while( bytesRecv = recv( clisock, buf, 2000, 0 ) > 0){

	printf("in the while statement\n");
	
	printf("received a message of size %d\n", bytesRecv);
	printf("%s\n", buf);
	
	send(clisock, buf, 2000, 0);
	//strcpy(buf, "");
	memset(buf, 0, sizeof(buf));

}

// close socket, once finished
close( servsock );
printf("socket closed\n");
return 0;
}