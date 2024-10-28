#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// Signal handler for SIGINT (Control-C) in child
void handle_sigint(int sig) {
    printf("Child process received SIGINT, exiting...\n");
    exit(0);
}

// Signal handler for SIGTSTP (Control-Z) in child
void handle_sigtstp(int sig) {
    printf("Child process received SIGTSTP, stopping...\n");
    kill(getpid(), SIGSTOP); // stop child
}

int main(int argc, char **argv) {
    pid_t pid;
    int status;

    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        exit(-1);
    }

    pid = fork();
    if (pid == 0) { /* this is child process */
        //signal handlers for SIGINT and SIGTSTP
        signal(SIGINT, handle_sigint);
        signal(SIGTSTP, handle_sigtstp);

        execvp(argv[1], &argv[1]);
        printf("If you see this statement then execvp failed ;-(\n");
        perror("execvp");
        exit(-1);
    } else if (pid > 0) { /* this is the parent process */
        printf("Parent process waiting for child to terminate\n");
        
        // Wait for the child process to terminate
        wait(&status);
        if (WIFEXITED(status)) { /* child process terminated normally */
            printf("Child process exited with status = %d\n", WEXITSTATUS(status));
        } else { //child doesnt terminate normally
            printf("Child process did not terminate normally!\n");
        }
    } else { // if forking fails
        perror("fork");
        exit(EXIT_FAILURE);
    }

    printf("[%ld]: Exiting program .....\n", (long)getpid());
    return 0;
}
