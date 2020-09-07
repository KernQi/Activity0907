#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>

int collectedChildren = 0;
int printFlag = 0;
int doneFlag = 0;

// Prototypes (forward declarations) for the functions below
void setupSignalHandler(int signal, void (*handler)(int));
void childHandler(int signal);
void termHandler(int signal);
void usr1Handler(int signal);

int main(int argc, char **argv) {
	int childID;

	//TODO
	//call the setupSignalHandler function with the appropriate
	//handler for each signal: SIGCHLD, SIGUSR1, and SIGTERM


	printf("My pid is: %d\n", getpid());

	while(!doneFlag) {
		if(printFlag){
			printf("Number of finished children: %d\n", collectedChildren);
		}

		childID = fork();

		if(childID < 0){
			perror("fork failed\n");
			return EXIT_FAILURE;
		}
		if(childID == 0) {
			return EXIT_SUCCESS;
		}

		// Q1: Why do we need to call sleep?
		// Q2: Why do we need the if statement?
		while(sleep(1) != 0) {
			sleep(1);
		}
	}

	printf("All done!\n");
	return EXIT_SUCCESS;
}

//sets up a sigaction struct then calls the sigaction syscall
//to register the handler function for the given signal
void setupSignalHandler(int signal, void (*handler)(int)) {
	//TODO
	//create a sigaction struct -
	//zero the memory in the sigaction struct -
	//set the sa_handler field -
	//call sigaction & detect errors
	struct sigaction* ourSigaction = malloc(sizeof(struct sigaction));

	memset(ourSigaction, 0, sizeof(sigaction));

	ourSigaction->sa_handler = handler;

	if(sigaction(signal, ourSigaction, NULL) == -1){
		perror("error message.");
	}

	free(ourSigaction);
}

//reaps a a child process
void childHandler(int signal) {
	int status;

	//Q3: Why do we need to wait AFTER receiving a child signal?
	//Q4: Why do we need to call waitpid in a loop?
	while(waitpid(-1, &status, WNOHANG) != -1) {
		collectedChildren++;
	}
}

//sets a flag that will cause the program to exit on the next loop
void termHandler(int signal) {
	doneFlag = 1;
}

//toggles printing of the number of reaped children
void usr1Handler(int signal) {
	printFlag = !printFlag;
}
