#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

int main( int args, char argv[] ) {

// create socket
int servsock = socket( AF_INET, SOCK_STREAM, 0 );

 printf("debug0\n");
 
// create server socket address
struct sockaddr_in servaddr;
memset( &servaddr, 0, sizeof(servaddr) );
 printf("debuggery\n"); 
servaddr.sin_family      = AF_INET;
 printf("1\n");
servaddr.sin_port        = htons( 5000 );
 printf("2\n");
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // inet_addr( INADDR_ANY );

 printf("debug1\n");
 
// configure servsock using servaddr
int err = bind( servsock, (struct sockaddr *)&servaddr, sizeof(servaddr) );

 printf("debug2\n");
 
// make servsock listen for incoming client connections
listen( servsock, 5 );

 printf("debug3\n");
 
// accept client connection and log connection information
struct sockaddr_in cliaddr;
int cliLen;
cliLen = sizeof( struct sockaddr_in );
int clisock = accept( servsock, (struct sockaddr *)&cliaddr, &cliLen );

char buf[10];
recv( servsock, (void *)buf, 10, 0 );
printf(buf);



// close socket, once finished
close( servsock );

 return 0;

}
