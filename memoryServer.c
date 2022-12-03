#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define SERVER_PORT 1084 // 108x port ranvge
#define BUF_SIZE 256

// We make this a global so that we can refer to it in our signal handler
int serverSocket; 

/*
  We need to make sure we close the connection on signal received, otherwise we have to wait 
  for server to timeout.
*/
void closeConnection() {
  printf("\nClosing Connection with file descriptor: %d \n", serverSocket);
  close(serverSocket);
  exit(1);
}

int main(int argc, char *argv[]) {
    int connectionToClient, bytesReadFromClient;
   
    // These are the buffers for sending data to the client and receiving
    char sendLine[BUF_SIZE];
    char receiveLine[BUF_SIZE];
    
    // Create a server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family      = AF_INET;
    
    // INADDR_ANY means we will listen to any address
    // htonl and htons converts address/ports to network formats
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port        = htons(SERVER_PORT);
    
    // Bind to port
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
      printf("Unable to bind to port just yet, perhaps the connection has to be timed out\n");
      exit(-1);
    }

    // Before we listen, register for Ctrl+C being sent so we can close our connection
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = closeConnection;
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    
    // Listen and queue up to 10 connections
    listen(serverSocket, 10);
    
    while (1) {
        /*
          Accept connection (this is blocking)
          2nd parameter you can specify connection
          3rd parameter is for socket length
        */
        connectionToClient = accept(serverSocket, (struct sockaddr *) NULL, NULL);
        
        // Read the request that the client has
        while ( (bytesReadFromClient = read(connectionToClient, receiveLine, BUF_SIZE)) > 0)
	{
            // Need to put a NULL string terminator at end
            receiveLine[bytesReadFromClient] = 0;
            
            // Show what client sent
            printf("Received From Client: %s\n", receiveLine);
            
            // Print text out to buffer, and then write it to client (connectionToClient)
        //    snprintf(sendLine, (sizeof(sendLine)+11), "Memory: %s\n", receiveLine);
          
	    snprintf(sendLine, (sizeof(sendLine)+ 5),  "0:"); // "%ld:%s", strlen(receiveLine), receiveLine);
	    printf("Sending to client: %s", sendLine);
            write(connectionToClient, sendLine, strlen(sendLine));

            
            // Zero out the receive line so we do not get artifacts from before
            bzero(&receiveLine, sizeof(receiveLine));
            close(connectionToClient);
        }
    }
}



