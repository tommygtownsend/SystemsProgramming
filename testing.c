#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

// Function to split a string into an array of arguments
void createarray(char *buf, char **array) {
    int i, count, len;
    len = strlen(buf);
    buf[len-1] = '\0'; /* replace last character (\n) with \0 */
    for (i = 0, array[0] = &buf[0], count = 1; i < len; i++) {
        if (buf[i] == ' ') {
            buf[i] = '\0';
            array[count++] = &buf[i + 1];
        }
    }
    array[count] = (char *)NULL; // NULL-terminate the array
}

int main(int argc, char **argv) {
    pid_t pid; // Variable to store process ID
    int status; // Variable to store the exit status of the child process
    char line[BUFSIZ], buf[BUFSIZ], *args[BUFSIZ]; // Buffers for input line, command, and arguments
    time_t t1, t2; // Variables to store start and end time

    // Check if the correct number of arguments is provided
    if (argc < 2) {
        printf("Usage: %s <commands file>\n", argv[0]);
        exit(-1);
    }

    // Open the commands file for reading
    FILE *fp1 = fopen(argv[1], "r");
    if (fp1 == NULL) {
        printf("Error opening file %s for reading\n", argv[1]);
        exit(-1);
    }

    // Open a log file for writing output from the parent process
    FILE *fp2 = fopen("output.log", "w");
    if (fp2 == NULL) {
        printf("Error opening file output.log for writing\n");
        exit(-1);
    }

    // Read commands line by line from the input file
    while (fgets(line, BUFSIZ, fp1) != NULL) {
        strcpy(buf, line); // Save line read into buf
        createarray(line, args); // Parse the line into arguments
        
        time(&t1); // Get the start time
        pid = fork(); // Fork a new process

        if (pid == 0) { // Child process
            // Prepare filenames for stdout and stderr redirection
            char outFile[BUFSIZ], errFile[BUFSIZ];
            snprintf(outFile, sizeof(outFile), "%d.out", getpid()); // Create output filename
            snprintf(errFile, sizeof(errFile), "%d.err", getpid()); // Create error filename

            // Open output and error files
            FILE *out = fopen(outFile, "w");
            FILE *err = fopen(errFile, "w");
            if (out == NULL || err == NULL) {
                perror("Error opening output or error file");
                exit(EXIT_FAILURE); // Exit if file opening fails
            }

            // Redirect stdout and stderr to the opened files
            dup2(fileno(out), STDOUT_FILENO); // Redirect stdout to out file
            dup2(fileno(err), STDERR_FILENO); // Redirect stderr to err file

            // Close file pointers as they are no longer needed in the child
            fclose(out);
            fclose(err);

            // Execute the command using execvp
            execvp(args[0], args); // Replace child process with the command
            perror("exec"); // Only reached if exec fails
            exit(-1); // Exit if exec fails
        } else if (pid > 0) { // Parent process
            printf("Child started at %s", ctime(&t1)); // Log child start time
            printf("Wait for the child process to terminate\n");
            wait(&status); // Wait for the child process to terminate
            time(&t2); // Get the end time
            printf("Child ended at %s", ctime(&t2)); // Log child end time

            // Check if the child terminated normally
            if (WIFEXITED(status)) { 
                printf("Child process exited with status = %d\n", WEXITSTATUS(status)); // Log exit status
            } else { 
                printf("Child process did not terminate normally!\n"); // Log abnormal termination
            }

            // Prepare to log command execution time
            buf[strlen(buf) - 1] = '\t'; // Replace \n with \t
            strcat(buf, ctime(&t1)); // Append start time to command log
            buf[strlen(buf) - 1] = '\t'; // Replace \n added by ctime with \t
            strcat(buf, ctime(&t2)); // Append end time
            fprintf(fp2, "%s", buf); // Write log to output.log
            fflush(fp2); // Ensure all output is written
        } else { // Fork error
            perror("fork"); // Print error message
            exit(EXIT_FAILURE); // Exit on fork failure
        }
    }

    // Close opened files
    fclose(fp1);
    fclose(fp2);
    printf("[%ld]: Exiting main program .....\n", (long)getpid()); // Log exiting main program

    return 0; // Exit program
}
