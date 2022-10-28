#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
int c=0;
/*struct job{
        int pid;
        char NameProg[32];
};*/

void * background(void * pid) {
        int status=0;
        int* pidf= (int*) pid;
        waitpid(*pidf,&status,0);
        return pid;
}

int main (int argc, char * argv[]) {
  char buffer[32];
  //struct job job[5];
  int pidList[5];
  char programList[5][32];
  int i=0,j, flag=0, status=0, pid;
  pthread_t thread;
  while(1) {
    printf("\nMyShell>>> ");
    fgets(buffer, 32, stdin);

    for(i; i < 32; i++){
      if (buffer[i] == '\n') {buffer[i] = '\0'; break;}
    }
    if (buffer[i-1]=='&') {flag=1; buffer[i-2]='\0';i=i-2;}
    if(strcmp(buffer,"jobs")==0){
            printf("\nThe current background jobs are:\n");
       printf("PID\t\tProgram\n");
        for (j=0; j<c; j++){
        printf("%d\t\t%s\n",pidList[j],programList[j]);
        }
    }
    else{
    pid= fork();
    if ( pid != 0) { //parent //DONTTTT
      // wait when there is no &  meaning flag is 0
      if (flag ==0){ //process runs in foreground
              printf("\nFOREGROUND\n");
        waitpid(pid,&status,0); //we wait for it to complete before we prompt the shell again
      }
      else{
              printf("\nBACKGROUND\n");

              //add table
                pidList[c]=pid;
                strcpy(programList[c],buffer);
                        c++;
              //pthread => waitpid => monitor the background process
              //no join
              //remove from table
              //
               pthread_create(&thread, NULL , background , (void *)&pid);
                c--;

      }}
    else { //child

      char * arguments[] = {buffer, NULL};
      char * envp[] = {NULL};
      execve(buffer, arguments, envp);
    }
  }
    }
}
