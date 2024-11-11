//to compile: gcc -o lab11 lab11.c
//to run: ./lab11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// I think this has been the easiest one yet. I got it done fast enough that I 
// kept second guessing myself as to if it is right or not. It can do everything
// in the terminal similar to the example. It pipes using popen and pclose, 
// has an infinite loop, to take care of the interface, and quits with 
// "quit"
#define MAX_CMD_LENGTH 1024 //we'll make it big enough to fit most any command

int main(int argc, char **argv) {
    char command[MAX_CMD_LENGTH];
    FILE *fp;
    char buffer[MAX_CMD_LENGTH];

    // while loop for ui
    while (1) {
        
        printf("Enter command: ");
        
        // read your command
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        // remove the newline character at the end of the command string
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "quit") == 0) {
            printf("Exiting program...bye!\n");
            break;
        }

        // open a pipe and execute the command with popen
        fp = popen(command, "r");
        if (fp == NULL) {
            perror("popen");
            exit(EXIT_FAILURE);
        }

        // read and print output
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
        }

        // close the pipe
        if (pclose(fp) == -1) {
            perror("pclose");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}