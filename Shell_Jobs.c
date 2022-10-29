#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>


struct job {
	int PID;
	char program[32];
};


struct job joblist[5];
int status = 0; 
int counter = 0; 
int PID; 
char test[] = "jobs";


void * run_process(void * process)
{
int * p = (int *)process;
joblist[counter-1].PID = -PID; 
waitpid(*p, &status, 0);
}


void main() {
	char input[32];
	int input_length, in_background;
	pthread_t thread1;//, thread2, thread3, thread4, thread5;

	while(1) {
	// Prompts the user for input and resets the in_background variable.
		in_background = 0;
		printf("Please input a program to run: ");
		fgets(input, 32, stdin);

		
		// Removes newline characters and saves the length of the input.
		for(int x = 0; x < 32; x++) {
			if(input[x] == '\n') {
				input[x] = '\0';
				input_length = x-1;
				break;
			}
		}
		

		// Checks for a '&' with a space before it at the end of the input.
		if(input[input_length] == '&') {
			in_background = 1;
			input[input_length] = '\0';
	
		}

		
		char * arg1[] = {input, NULL};
		char * arg2[] = {NULL};

//Fork to handle process and continue prompt

			PID = fork();

			if(PID == 0)   //Child process
			{       
                                     //Tests if 'jobs' was input
			       if(strcmp(input, test) == 0 )
				{
                                 
    				     printf("PID   Program \n");
  				     for(int i = 0; i < 5; i++)
				     {   
				        if(joblist[i].PID > -56500500000)
				        {
      			                   printf("%d  %s \n", joblist[i].PID, joblist[i].program);
			                }
				
				     }
				}
				else   //Execve to run input file
				{
				execve(input, arg1, arg2);
				}
			}
			else   //Parent process
			{          
			      
				if(in_background == 1)
				{
				int* pid = &PID;
                                 
                                //Add background process to jobs list
                        	strcpy(joblist[counter].program, input);
			        joblist[counter].PID = PID; 
                                  
       				//Create thread to wait for background process
			        pthread_create(&thread1, NULL, run_process, (void*)pid);  

				counter++; //Increment job counter
		            	}
                                else    //Simply wait if foreground process 
		                {
			           waitpid(PID, NULL, 0);
			        }

			}
		}
	
	} //End while

}//End main
