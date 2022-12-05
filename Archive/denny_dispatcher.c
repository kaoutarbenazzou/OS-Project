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
#include <sys/sysinfo.h>
#include <arpa/inet.h>
#define RESOURCE_SERVER_PORT 1089 // Change this!
#define BUF_SIZE 256

// We make this a global so that we can refer to it in our signal handler
int serverSocket;
int thread_count; 

char errLine[BUF_SIZE]; //Used to send error messages to client 
//char sendLine[BUF_SIZE]; //  Changed to local variable within save, read, and delete 

/*
 We need to make sure we close the connection on signal received, otherwise we have to wait
 for server to timeout.
 */
void closeConnection() {
    printf("\nClosing Connection with file descriptor: %d \n", serverSocket);
    close(serverSocket);
    exit(1);
}


// Function for communicating with a server:
int clientSend(int portNumber, char * message, char * response) {
	

	// Create server socket:
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Unable to create socket.\n");
		return -1;
	}


	// Set up connection:
	struct sockaddr_in serverAddress;
	bzero(&serverAddress, sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber); //Connects to server based off port


	// Converts IP address to binary:
	if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0){
		printf("Unable to convert IP for server address.\n");
		return -1;
	}


	// Connects to server:
	if (connect(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
	{
		printf("Unable to connect to server.\n");
	}


	// Sends message to server:
	write(serverSocket, message, strlen(message));


	// Fills out the response string:
	// TODO: Add an if-then statement to check if the response pointer is NULL.
	response = "Responding";
	printf("Responding: %s\n", response);	

}




// File command functions (Denny U.):
// Assigned to Dennis Ulichney.
void save(char * receiveLine)
{

	int receiveEnd = strlen(receiveLine) - 1, c = 0;
	char fileInfo[BUF_SIZE - 6], sendLine[BUF_SIZE];
	

	// Collects the file information from the recieved string:
	for(int i = 5; i < receiveEnd; i++) {
		fileInfo[c] = receiveLine[i];
		c++;
	}

//char declared[256]; 
	// Writes and sends the save file command to the file server:
	snprintf(sendLine, sizeof(sendLine), "write %s", fileInfo);
	clientSend(1085, sendLine, NULL);

}


// Assigned to Sonny Smith.
void read_file(char receiveLine[])
{

   	char fileLine[BUF_SIZE], sendLine[BUF_SIZE]; 

	for(int i = 0; i < (BUF_SIZE) - 1; i++)  //Removes the newline character from the filename
	{
		if(receiveLine[i] == '\n')// || //receiveLine[i+5] == ' ' || receiveLine[i+5] == '\0'  ) 
		{
		break; 
		}
		else
		{
		fileLine[i] = receiveLine[i]; 
		}
	}
/*   WIP
	if(clientSend(1084, sendLine, NULL) != NULL)
	{	
	//	filename = response;
	}
	else if( clientSend(1085, sendLine, NULL) != NULL)
	{
		//filename = response;	
	}
	else
	{
	printf("Error file not found!");
	}
*/
	clientSend(1085, fileLine, sendLine); 	
     snprintf(sendLine, sizeof(sendLine), "%s", sendLine); //Sends a response to the client 

    // snprintf(sendLine, sizeof(sendLine), "%d:%s", receive_size, filename); 

}


// Assigned to Kaoutar Ben Azzou.
void delete(char * receiveLine)
{

char sendLine[BUF_SIZE];  //Variable used with snprintf to respond to client

}


// The thread function seems like it is meant to handle recieving and sending messages to the other components (Denny U.).
void * processClientRequest(void * request) {
    
    int connectionToClient = *(int *)request;  

    char receiveLine[BUF_SIZE];
  // char sendLine[BUF_SIZE];   //Changed to local in save/read/delete
  strcpy(errLine, "DEFAULT");
    
    int bytesReadFromClient = 0;


    // Read the request that the client has
    while ( (bytesReadFromClient = read(connectionToClient, receiveLine, BUF_SIZE)) > 0){
        

	// Need to put a NULL string terminator at end
        receiveLine[bytesReadFromClient] = 0;
        
        // Show what client sent
        printf("Received: %s\n", receiveLine);

	
	// Check for command from client (Denny U.):

	if (strstr(receiveLine, "save ") != NULL) {
		save(receiveLine);
	}

	else if (strstr(receiveLine, "read ") != NULL) {
		read_file(receiveLine);
	}
	else if (strstr(receiveLine, "delete ") != NULL) {
		delete(receiveLine);
	}

	else {
		strcpy(errLine, "Please enter a valid comand.");
	        write(connectionToClient, errLine, strlen(errLine)); 
	}

	//	snprintf(errLine, sizeof(errLine), "%s", errLine);
      
        // Print text out to buffer, and then write it to client (connfd)
//       snprintf(sendLine, sizeof(sendLine), "true");  //Remove so that read,save, or delete functions handle output to client
      
//        printf("Sending: %s\n", errLine);  //Only needed for troubleshooting 
//       write(connectionToClient, errLine, strlen(errLine)); //Redundant if used in save/read/delete.
        
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
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1)
    {
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
    
    while (1) 
    {
        /*
         Accept connection (this is blocking)
         2nd parameter you can specify connection
         3rd parameter is for socket length
         */
        connectionToClient = accept(serverSocket, (struct sockaddr *) NULL, NULL);
        

	// Updates thread limit:
	int thread_limit = get_nprocs() * 2;

	if(thread_count < thread_limit) 
	{
		thread_count++;

        	// Kick off a thread to process request
        	pthread_t someThread;
        	pthread_create(&someThread, NULL, processClientRequest, (void *)&connectionToClient);
	}

	else 
	{
		// TODO: How can I inform the client that the thread limit has been reached?    
	printf("Thread Limit Reached\n"); 
        strcpy(errLine, "Thread Limit Reached\n"); 
	write(connectionToClient, errLine, strlen(errLine));   // TODO: Test if thread limit correctly informs client
	}

    }
}

