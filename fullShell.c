#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

//Collaborated with Dennis U. & Kaoutar A. 



void * bgProcess(void * file)
    {
  printf("Thread Reached \n");
       char * f =  ((char *)file);
       char * arguments[] = {f, "anArgument", NULL};
        char * envp[] = {NULL};
        execve(f, arguments, envp );

    }




int main(int argc, char *argv[]  )
{
int status = 0;
char fileIn[32]; 
int background = 0; 
while(1)
{
     printf("Prompt> ");
     fgets(fileIn, 32, stdin);


 int x = 0;                            
  while(x < 32 && fileIn[x] != '\0')   //Loop that removes newLine character from string
    {
     if(fileIn[x] ==  '\n') 
     {
	fileIn[x] = '\0'; 
     }
     else if( (fileIn[x] == '&' && fileIn[x-1] == ' ') )  //Used to detect  'fileName &'
     {
     background = 1;
     fileIn[x-1] = '\0'; 
/*     
    void * file = &fileIn;
    pthread_t thread1;
    pthread_create(&thread1, NULL, bgProcess, (void*)file);
*/

     }
     else
     {
     x++;
     }


    }

int pid = fork();

if( pid != 0) //Parent Actions
  {
//          if(background != 1)
//	  {
	  waitpid(pid, &status, 0); //Ensures that prompt returns after program is run. 
//	  }
	  printf("Status: %ls \n ", &status); 
  }  

else if(background == 1)   //Child Actions In background
  {
  /*
    void * file = &fileIn;
    pthread_t thread1;
    pthread_create(&thread1, NULL, bgProcess, (void*)file);
   */
  }
  else   //Child Actions
  {     

        printf("Child Process Reached \n"); 
        char * arguments[] = {fileIn, "anArgument", NULL};
        char * envp[] = {NULL};
        execve(fileIn, arguments, envp );     //Replaces current code(child) with the code of fileIn


  }





}//End prompt while

   

}//End program

