#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/sysinfo.h>

#define RESOURCE_SERVER_PORT 1089 // Change this!
#define BUF_SIZE 256

// We make this a global so that we can refer to it in our signal handler
int serverSocket;
int thread_count; 

char errLine[BUF_SIZE]; //Used to send error messages to client
//char sendLine[BUF_SIZE];

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
void clientSend(int portNumber, char * message, char * response) {
	int bytesRead, processSocket;


	// Create server socket:
	if ((processSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Unable to create socket.\n");
		return;
	}


	// Set up connection:
	struct sockaddr_in serverAddress;
	bzero(&serverAddress, sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);


	// Converts IP address to binary:
	if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0){
		printf("Unable to convert IP for server address.\n");
		return;
	}


	// Connects to server:
	if (connect(processSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
		printf("Unable to connect to server with address: %d\n", portNumber);
		
	close(processSocket);
		}
	

printf("Sending to server: %s\n", message);
	// Sends message to server:
	write(processSocket, message, strlen(message));

printf("Response Before: %s \n", response);    //Used to test server response
	// Reads server response:
	if (response != NULL) {
		
		//TODO: Why is this a while loop instead of an if-then statement?
		while ((bytesRead = read(processSocket, response, BUF_SIZE)) > 0) {
			response[bytesRead] = 0;
			//TODO: Do I need a print function here?
			break;
		}
	}

printf("Response After: %s \n", response);   //Used to test server response
	// Close server connection:
	close(processSocket);
}




// File command functions (Denny U.):
// Assigned to Dennis Ulichney.
void save(char * receiveLine) {
	printf("1--%s--\n", receiveLine);
	int receiveEnd = strlen(receiveLine) - 1, c = 0;
	char fileInfo[250], sendLine[BUF_SIZE];
	
	printf("2--%s--\n", receiveLine);

	// Collects the file information from the recieved string:
	for(int i = 5; i < receiveEnd; i++) {
		fileInfo[c] = receiveLine[i];
		c++;
	}
	printf("3--%s--\n", receiveLine);


	// Writes and sends the save file command to the file server:
	// TODO: Why is there a problem with the size of fileInfo?
	snprintf(sendLine, sizeof(sendLine), "write %s", fileInfo);
	printf("4--%s--\n", receiveLine);
	printf("4.5 Goign to send--%s--\n", sendLine);
	clientSend(1085, sendLine, NULL);
	printf("5--%s--\n", receiveLine);
}


// Assigned to Sonny Smith.
void read_file(char * receiveLine, int * client) {

   	char fileLine[BUF_SIZE - 5], sendLine[BUF_SIZE], serverLine1[BUF_SIZE], serverLine2[BUF_SIZE]; 

	//strcpy(sendLine, receiveLine) 

	for(int i = 0; i < (BUF_SIZE) - 1; i++)  //Removes the newline character from the filename and extracts just the filename for memory server
	{
		if(receiveLine[i] == '\n')// || //receiveLine[i+5] == ' ' || receiveLine[i+5] == '\0'  ) 
		{
		break; 
		}
		else
		{
		fileLine[i] = receiveLine[i + 5];   //Extracts the file contents  
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
	snprintf(serverLine1, sizeof(serverLine1), "load %s", fileLine);
printf("fielLine: %s\n", fileLine);
	snprintf(serverLine2, (sizeof(serverLine2) ) , "read %s", fileLine);  
printf("fileLine after: %s\n", fileLine);
	clientSend(1084, serverLine1, sendLine); //Tests if file is at the memory server 
	printf("Send line is: %s\n", sendLine);
	if(strcmp(sendLine, "0:") == 0 || sendLine == NULL  ) 
	{
		printf("File not found in memory! \n");
		clientSend(1085, serverLine2, sendLine); //Tests if file is at the file server
		if( strcmp(sendLine, "0:") == 0 || sendLine == NULL  )
		{
		strcpy(sendLine, "ERROR: File Not Found!\n");
		}
	}
	
     printf("After client send we have: %s\n ", sendLine);
 //   snprintf(sendLine, sizeof(sendLine), "%s", sendLine); //Prepares the response to the client 
    write(*client, sendLine, strlen(sendLine));  //Sends response to client

    // snprintf(sendLine, sizeof(sendLine), "%d:%s", receive_size, filename); 

}


// Assigned to Kaoutar Ben Azzou.
char sendLine1[BUF_SIZE+10];
char sendLine2[BUF_SIZE+10];


void delete(char receiveLine[]) {

  char filename[BUF_SIZE];
  char * ptr;

        int size = strlen(receiveLine);
        int i,p=0,j;
    for (i=0;i<size;i++){
        if (receiveLine[i]==' '){
            break;
        }
    }

	j=i+1;
	
	
 	while(receiveLine[j]!='\n'){
            filename[p++]=receiveLine[j++];
        }
	
    filename[j]='\0';
    }

snprintf(sendLine1, sizeof(sendLine1), "delete %s", filename);
clientSend(1085,sendLine1,NULL);

snprintf(sendLine2, sizeof(sendLine2), "rm %s", filename);
clientSend(1084,sendLine2,NULL); 
}



// The thread function seems like it is meant to handle recieving and sending messages to the other components (Denny U.).
void * processClientRequest(void * request) {
    
    int connectionToClient = *(int *)request;  

    char receiveLine[BUF_SIZE];//, sendLine[BUF_SIZE];
 
    
    int bytesReadFromClient = 0;


    // Read the request that the client has
    while ( (bytesReadFromClient = read(connectionToClient, receiveLine, BUF_SIZE)) > 0){
        

	// Need to put a NULL string terminator at end
        receiveLine[bytesReadFromClient] = 0;
        
        // Show what client sent
//        printf("Received: %s\n", receiveLine);

	
	// Check for command from client (Denny U.):

	if (strstr(receiveLine, "save ") != NULL)
	{
//		if( strcmp(receiveLine[5], " ") == 0)
//		{
		save(receiveLine);
//		}
//		else
//		{
//		printf("Incorrect Command Syntax!\n"); 
//		}
	}
	else if (strstr(receiveLine, "read ") != NULL)
	{
		read_file(receiveLine, &connectionToClient);
	
	}
	else if (strstr(receiveLine, "delete ") != NULL)
	{
		delete(receiveLine);
		
	}

	else {
		strcpy(errLine, "Please enter a valid comand.");
	        write(connectionToClient, errLine, strlen(errLine));
	}

      
        // Print text out to buffer, and then write it to client (connfd)
//       snprintf(errLine, sizeof(errLine), "true");  //Remove so that read,save, or delete functions handle output to client
      
    //    printf("Sending: %s\n", errLine); 
  //      write(connectionToClient, sendLine, strlen(errLine));
        
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

