#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

//Collaborated with Dennis U. & Kaoutar A. 

int jobCount = 0;
int jobBackup = 0; 
int pid = 0;
int status = 0;
int processNum = 0;  

struct jobs{
	int PID;
	char Program[32];
};

struct jobs jobList[5]; //Jobs array



void * bgProcess(void * process) 
     {
    int *p = (int*)process;      
	 waitpid(*p, &status, 0);


//    Troubleshooting Code	
// printf("PID: %d \n ", pid);
// jobList[jobCount].PID = -22;          
// jobList[jobBackup].PID = jobList[jobCount].PID;          
//printf("PID After: %d \n ", jobList[jobCount].PID);
//          jobCount--; 
//	 return jobList;
//	processNum = 0; 
//  jobCount--; 
// pthread_exit(&thread1);

     }



int main(int argc, char *argv[]) {

	char fileIn[32]; 

//	int processNum = 0; 
//	struct jobs jobList[5]; 

	// This outer while loop allows for continuous prompting.

	while(1) {
     		printf("Prompt> ");
     		fgets(fileIn, 32, stdin);
		int x = 0;
               
		//Loop that removes newLine character from string.           
  		while(x < 32 && fileIn[x] != '\0')
		{
     			
			if(fileIn[x] ==  '\n')
			{
			
			fileIn[x] = '\0';
		//	processNum = 0; 

			}

			//Used to detect  'fileName &'.
			else if( (fileIn[x] == '&' && fileIn[x-1] == ' ') ) 
			{
      				processNum = 1;
     				fileIn[x-1] = '\0'; 
    			}
                             
     			else 
			{
     				x++;
     			}
    		}

		char test[] = "jobs";
		if(strcmp(fileIn, test) == 0)
		{
   			processNum = 2;
		}

		//strcpy(jobList[jobCount].Program, fileIn); //Adds the name of the input file to the job list 
	
                pid = fork();   //Initiates Fork
	        //jobList[jobCount].PID = pid;              //Adds the pid of the forked process 
                
              //  if(jobCount < 6)
	//	{
         //          jobCount++;     //Increments jobCount
	//	}


		//Parent Actions
		if( pid != 0)
		{

			//Ensures that prompt returns after program is run. 
                        if(processNum  == 1)
			{
  			  int * bpid = &pid;
  			  pthread_t thread1;
                          strcpy(jobList[jobCount].Program, fileIn); //Adds the name of the input file to the job list 
	                  jobList[jobCount].PID = pid;              //Adds the pid of the forked process 
		          pthread_create(&thread1, NULL, bgProcess, (void*)bpid);
		        
			jobBackup = jobCount;    
                          jobCount++;     //Increments jobCount


		        // pthread_exit(&thread1);
		        }
                        else
		        { 
                          waitpid(pid, &status, 0);
		        }
	  			
  		}
  		else
		{

		        if(processNum != 2)
                        {
			        //Foreground process
			//	printf("FOREGROUND\n"); 
  			        char * envp[] = {NULL};
                                char * arguments[] = {fileIn, NULL, NULL}; 
				execve(fileIn, arguments, envp );
                        }
		        else
		        {
				//Job Process
    				printf("PID   Program \n");
  				for(int i = 0; i < jobCount; i++)
				{ 
				   if(jobList[i].PID > 0)
				   {
      			           printf("%d HELLOOOOOOO  %s \n", jobList[i].PID, jobList[i].Program);
			           }
				
				}

	          	}

		}

  		

	}//End prompt while

}//End program


