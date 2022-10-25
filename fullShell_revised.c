#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

//Collaborated with Dennis U. & Kaoutar A. 

int jobCount = 0; 

struct jobs{
	int PID;
	char Program[32];
};

void * bgProcess(void * file) {

	int status = 0;
	//Ensures that prompt returns after program is run. 
	if( fork() != 0) {
		waitpid(1, &status, 0);
	}

	else {
		printf("Thread Reached \n");
       		char * f =  ((char *)file);
       		char * arguments[] = {f, "anArgument", NULL};
        	char * envp[] = {NULL};
        	execve(f, arguments, envp );
	}
}




int main(int argc, char *argv[]) {

	int status = 0;
	char fileIn[32]; 
	int processNum = 0; 
	struct jobs jobList[5]; 

	// This outer while loop allows for continuous prompting.
	while(1) {
     		printf("\nPrompt> ");
     		fgets(fileIn, 32, stdin);
		int x = 0;
               
		//Loop that removes newLine character from string.           
  		while(x < 32 && fileIn[x] != '\0') {
     			if(fileIn[x] ==  '\n') {
				fileIn[x] = '\0';
			}
     
			//Used to detect  'fileName &'.
			else if( (fileIn[x] == '&' && fileIn[x-1] == ' ') ) {
      				processNum = 1;
     				fileIn[x-1] = '\0'; 
     
				void * file = &fileIn;
  				pthread_t thread1;
   				pthread_create(&thread1, NULL, bgProcess, (void*)file);
    			}

     			else {
     				x++;
     			}
    		}

		char test[] = "jobs";
		if(strcmp(fileIn, test) == 0) {
   			processNum = 2;
		}

		strcpy(jobList[jobCount].Program, fileIn); //Adds the name of the input file to the job list 
		int pid = fork();
		jobList[jobCount].PID = pid; 

		//Parent Actions
		if( pid != 0) {

			//Ensures that prompt returns after program is run. 
          		if(processNum != 1) {
	  			waitpid(pid, &status, 0);
	  		}

		//printf("Status: %ls \n ", &status); 
  		}

  		else {
  			char * arguments[] = {fileIn, "anArgument", NULL};
  			char * envp[] = {NULL};
  			void * file = &fileIn;
  			pthread_t thread1;

			switch(processNum) {
				//Foreground Process
      				case 0:

				//Replaces current code(child) with the code of fileIn
                                execve(fileIn, arguments, envp );
    				break;

				//Background Process
      				case 1:
  				pthread_create(&thread1, NULL, bgProcess, (void*)file);
  				pthread_exit(&thread1);
  				break;

				//Job Process
    				case 2:
    				printf("PID   Program \n");
  				for(int i = 0; i < sizeof(jobList); i++) {
      					printf("%d   %s \n", jobList[i].PID, jobList[i].Program);
				}

  				break; 

      				default:
         			printf("ERROR! Unknown process or file input!\n");
	 			break; 
      			}
		}

  		

	}//End prompt while

}//End program

