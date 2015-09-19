/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                               */
/*  Program:  Assignment 5                                       */
/*  Authors:   Kelly Maclauchlan  Katherine Beltran              */
/*              100927176            100939080                   */
/*  Date:     December 7 2014                                    */
/*                                                               */
/*  Purpose:  A program that acts as a server reciving messages  */
/*           from the client useing TCP protocalls and saving    */
/*          them to a log file, made with code given in the class*/
/*                                                               */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUF  64

FILE *logFile;
void cleanup(); 
void handleSigInt(int);
/* global variables of sockets*/
int myListenSocket, clientSocket; 

/*
 Function: Main
 Purpose: creates the log file, server and handles all actions of the server
 */
int main()
{
  FILE *configFile;
  char logName[MAX_BUF];
  char ip[MAX_BUF];
  int  port;
  char str[MAX_BUF];
  
  struct sockaddr_in  myAddr, clientAddr;
  int i, addrSize, bytesRcv;
  char buffer[30];

     
  //signal handler for cleanup
  signal(SIGINT, handleSigInt);
     
/* get IP address and port number from config file */
  if (!(configFile=fopen(".config","r"))) {
    printf("cannot read config file...\n");
    exit(1);
  }
  fscanf(configFile, "%s", ip);
  fscanf(configFile, "%d", &port);
  fclose(configFile);

/* create name of log file;  format is
    username followed by timestamp
    usernameYYYYMMDDHHMM.log */
  strcpy(logName, getenv("USER"));
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  sprintf(str, "%4d%02d%02d%02d%02d",
          tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
          tm.tm_hour, tm.tm_min);
  strcat(logName, str);
  strcat(logName, ".log");

/* open log file for writing */
  if (!(logFile=fopen(logName,"w"))) {
    printf("cannot write to file...\n");
    exit(1);
  }

/* set up connection, receive commands typed
   into cuShell process, and log them to the
   logFile */
    
    myListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (myListenSocket < 0) {
      printf("eek! couldn't open socket\n");
      exit(-1);
    }
     
    /* setup my server address */
    memset(&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddr.sin_port = htons((unsigned short) port);
    
    /* bind my listen socket */
    
    i = bind(myListenSocket,
             (struct sockaddr *) &myAddr,
             sizeof(myAddr));
    if (i < 0) {
      printf("eek! couldn't bind socket\n");
      exit(-1);
    }
    
    /* listen */
    i = listen(myListenSocket, 5);
    if (i < 0) {
      printf("eek! couldn't listen\n");
      exit(-1);
    }
    
    
    /* wait for connection request */
    addrSize = sizeof(clientAddr);
    
    clientSocket = accept(myListenSocket,
                          (struct sockaddr *) &clientAddr,
                          &addrSize);
    if (clientSocket < 0) {
      printf("eek! couldn't accept the connection\n");
      exit(-1);
    }
    
    /* read message from client and put it with the date time stap into the log file */
    while (1) {
      bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);
      buffer[bytesRcv] = 0;
      
        //calls cleanup and breaks out of loop
      if(strcmp(buffer,"exit") == 0){
        cleanup();
        break;
      }
      
      time_t t = time(NULL);
      struct tm tm = *localtime(&t);
      fprintf(logFile, "%4d-%02d-%02d-%02d:%02d:%02d",
                tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
                tm.tm_hour, tm.tm_min,tm.tm_sec);
      fprintf(logFile,": %s\r\n",buffer);

       
    }  
    
    if(strcmp(buffer, "exit") == 0){
      cleanup();
    }
    
    /* close sockets */
    cleanup(); 
  return 0;
}
/*Function: cleanup()
  Purpose: closes all sockets and the log file written to 
*/
void cleanup(){
  close(myListenSocket); 
  close(clientSocket); 
  fclose(logFile);
  exit(-1);
}
/*Function handleSigInt()
  Puropse: call cleanup when sig int is recived
*/
void handleSigInt(int i){
    cleanup();
}

