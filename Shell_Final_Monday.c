#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
int c, status = 0;
int pidList[5];
char programList[5][32];

void *background(void *pid)
{
    int status = 0;
    int *pidf = (int *)pid;
    waitpid(*pidf, &status, 0);
    for (int i = 0; i < c; i++)
    {
        pidList[i] = pidList[i + 1];
        strcpy(programList[i], programList[i + 1]);
    }
    c--;
    return pid;
}

int main(int argc, char *argv[])
{
    char buffer[32];
    // struct job job[5];
    int i, j, flag, pid;
    pthread_t thread;

    while (1)
    {
        flag = 0, i = 0;
        printf("\nMyShell>>> ");
        fgets(buffer, 32, stdin);

        for (i; i < 32; i++)
        {
            if (buffer[i] == '\n')
            {
                buffer[i] = '\0';
                break;
            }
        }
        if (buffer[i - 1] == '&')
        {
            flag = 1;
            buffer[i - 2] = '\0';
            i = i - 2;
        }
        if (strcmp(buffer, "jobs") == 0)
        {
            printf("\nThe current background jobs are:\n");
            printf("PID\t\tProgram          \n");
            for (j = 0; j < c; j++)
            {
                printf("%d\t\t%s\n", pidList[j], programList[j]);
            }
        }
        else
        {
            pid = fork();
            if (pid != 0)
            { // parent
                // wait when there is no &  meaning flag is 0
                if (flag == 0)
                {                             // process runs in foreground
                    waitpid(pid, &status, 0); // we wait for process to complete before we prompt the shell again
                }
                else
                {
                    // add table
                    pidList[c] = pid;
                    strcpy(programList[c], buffer);
                    c++;
                    pthread_create(&thread, NULL, background, (void *)&pid);
                }
            }
            else
            { // child

                char *arguments[] = {buffer, NULL};
                char *envp[] = {NULL};
                execve(buffer, arguments, envp);
            }
        }
    }
}
