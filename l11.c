#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMD_LENGTH 1024

int main(int argc, char **argv) {
    char command[MAX_CMD_LENGTH];
    FILE *fp;
    char buffer[MAX_CMD_LENGTH];

    // while loop for ui
    while (1) {
        
        printf("Enter command: ");
        
        // read command
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        // Remove the newline character at the end of the command string
        command[strcspn(command, "\n")] = '\0';

        // Check if the user wants to quit
        if (strcmp(command, "quit") == 0) {
            printf("Exiting program...bye!\n");
            break;
        }

        // Open a pipe and execute the command using popen
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