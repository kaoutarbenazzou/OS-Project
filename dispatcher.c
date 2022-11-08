#include <stdio.h>
#include <pthread.h>
#include <string.h>


// Assigned to Dennis Ulichney.
void save(char[] filename, int filesize, char[] contents) {
	// Thread function for saving files.
}


// Assigned to Sonny Smith.
char[] read(char[] filename) {
	// Thread function for reading files.
}


// Assigned to Kaoutar Ben Azzou.
void delete(char[] filename) {
	// Thread function for deleting files.
}


void main() {
	int thread_limit, thread_count = 0;
	// Should the input size be larger than 32?
	char input[32];
	pthread_t thread;

	while(1) {
		thead_limit = get_nprocs() * 2;

		
		// What should happen if the thread limit is changed to a value that is less than the thread count?
		if(thread_count < thread_limit) {


			// Gets user input:
			printf("\nPlease enter a file command:\n");
			fgets(input, 32, stdin);


			// Removes newline characters from the input:
			for(int i = 0; i < 32; i++) {
				if(input[i] == '\n') {
					input[i] = '\0';
					break;
				}
			}


			// Creates threads to handle saving files:
			if(strstr(input, "save") != NULL) {
				printf("save looks good!");
			}

			
			// Creates threads to handle reading files:
			else if(strstr(input, "read") != NULL) {
				printf("read looks good!");
			}


			// Creates threads to handle deleting files:
			else if(strstr(input, "delete") != NULL) {
				printf("delete looks good!");
			}


			// Informs the user that they did not enter a valid command:
			else {
				printf("Please enter a valid command.");
			}
		}


		// Stops new threads from being created if the limit has been reached:
		else {
			printf("\nPlease wait for a thread to finish.\n");
		}
	}
}
