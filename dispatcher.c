#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#define RESOURCE_SERVER_PORT 1083 // Change this!
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


// File command functions (Denny U.):
// Assigned to Dennis Ulichney.
void save(char recieveLine[]) {
	int file_server_connection, c = 5;
	char write_command[BUF_SIZE];


	// Establish a connection to the file server:
	
	// Creates a new socket:
   	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    	struct sockaddr_in serverAddress;
    	bzero(&serverAddress, sizeof(serverAddress));
    	serverAddress.sin_family = AF_INET;
	
    	// INADDR_ANY means we will listen to any address
    	// htonl and htons converts address/ports to network formats
    	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    	serverAddress.sin_port = htons(RESOURCE_SERVER_PORT);
		
    	// Bind to port
    	if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        	printf("Unable to bind to port just yet, perhaps the connection has to be timed out\n");
        	exit(-1);
	}

        file_server_connection = accept(serverSocket, (struct sockaddr *) NULL, NULL);


	// Write the save file command to the file server:
	write_command[0, 1, 2, 3, 4] = ["w", "r", "i", "t", "e"];

	for(int i = 4; strcmp(recieveLine[i], "\0") != 0; i++) {
		write_command[c] = recieveLine[i];
		c++;
	}


	// Sends the save file command to the file server:
	
	
}


// Assigned to Sonny Smith.
void read(char recieveLine[]) {
	char filename[BUF_SIZE];
	int i = 6, c = 0;

	
	// Gets the name of the file:
	while(strcmp(recieveLine[i], " ") != 0) {
		filename[c] = recieveLine[i];
		c++;
		i++;
	}


	sendLine = // Put the feedback for the client here.
}


// Assigned to Kaoutar Ben Azzou.
void delete(char recieveLine[]) {

}


// The thread function seems like it is meant to handle recieving and sending messages to the other components (Denny U.).
void * processClientRequest(void * request) {
    int connectionToClient = *(int *)request;
    char receiveLine[BUF_SIZE];
    char sendLine[BUF_SIZE];
    
    int bytesReadFromClient = 0;
    // Read the request that the client has
    while ( (bytesReadFromClient = read(connectionToClient, receiveLine, BUF_SIZE)) > 0) {
        // Need to put a NULL string terminator at end
        receiveLine[bytesReadFromClient] = 0;
        
        // Show what client sent
        printf("Received: %s\n", receiveLine);

	
	// Check for command from client (Denny U.):
	if (strstr(receiveLine, "save") != NULL) {
		save(recieveLine);
	}

	else if (strstr(recieveLine, "read") != NULL) {
		read(recieveLine);
	}

	else if (strstr(recieveLine, "delete") != NULL) {
		delete(recieveLine);
	}

	else {
		sendLine = "Please enter a valid command.";
	}

      
        // Print text out to buffer, and then write it to client (connfd)
        snprintf(sendLine, sizeof(sendLine), "true");
      
        printf("%Sending s\n", sendLine);
        write(connectionToClient, sendLine, strlen(sendLine));
        
        // Zero out the receive line so we do not get artifacts from before
        bzero(&receiveLine, sizeof(receiveLine));
        close(connectionToClient);
    }

    thread_count--;
}


// The main function seems to be designed to handle input from the client (Denny U.).
int main(int argc, char *argv[]) {
    int connectionToClient, bytesReadFromClient, thread_count = 0;
  
    // Create a server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family      = AF_INET;
    
    // INADDR_ANY means we will listen to any address
    // htonl and htons converts address/ports to network formats
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port        = htons(RESOURCE_SERVER_PORT);
    
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
        

	// Updates thread limit:
	int thread_limit = get_nprocs() * 2;


	if(thread_count < thread_limit) {
		thread_count++;

        	// Kick off a thread to process request
        	pthread_t someThread;
        	pthread_create(&someThread, NULL, processClientRequest, (void *)&connectionToClient);
	}

	else {
		// TODO: How can I inform the client that the thread limit has been reached?
	}
    }
}

