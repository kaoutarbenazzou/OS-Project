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

void * run_process(void * input) {
        char * arg1[] = {input, NULL};
        char * arg2[] = {NULL};
        int PID = fork();

        if(PID != 0) {
                execve(input, arg1, arg2);
        }
}

void main() {
        char input[32];
        int input_length, in_background;
        pthread_t thread1, thread2, thread3, thread4, thread5;
        struct job joblist[5];
        int PID;


        while(1) {
                // Prompts the user for input and resets the in_background variable.
                in_background = 0;
                printf("Please input a program to run.\n");
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


                // Handles foreground processes.
                if(in_background == 0) {
                        PID = fork();

                        if(PID != 0) {
                                execve(input, arg1, arg2);
                                waitpid(PID, NULL, 0);
                        }
                }


                // Handles background processes.
                else {

                }
        }
}
                                                                           