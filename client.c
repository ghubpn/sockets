#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

int main( int args, char argv[] ) {

// create socket
int clisock;
clisock = socket( AF_INET, SOCK_STREAM, 0 );


// create client socket address
struct sockaddr_in servaddr;
memset( &servaddr, 0, sizeof(servaddr) );
servaddr.sin_family      = AF_INET;
 servaddr.sin_port = htons(5000);
servaddr.sin_addr.s_addr = inet_addr("172.17.85.252");

 printf("yomamma\n");
 
// connect to server once accepted
connect( clisock, (struct sockaddr *)&servaddr, sizeof(servaddr) );

 printf("did i connect?\n");
 
// send "Hello"
char buf[10];
strcpy( buf, "Hello\n" );

 printf("string?\n");

 send( clisock, (void *)buf, strlen(buf), 0 );

 printf("omg!\n");

// close socket, once finished
close( clisock );

 printf("fuckyeahIcompile!!!\n");

 return 0;

}
