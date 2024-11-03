//To Compile: gcc -Wall -o -O lab10 lab10.c
//To Run:./lab10 file.txt

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>


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
    pid_t pid; 
    int status; 
    char line[BUFSIZ], buf[BUFSIZ], *args[BUFSIZ]; 
    time_t t1, t2; // start and end time

 
    if (argc < 2) {
        printf("Usage: %s <commands file>\n", argv[0]);
        exit(-1);
    }

  
    FILE *fp1 = fopen(argv[1], "r");
    if (fp1 == NULL) {
        printf("Error opening file %s for reading\n", argv[1]);
        exit(-1);
    }



    FILE *fp2 = fopen("output.log", "w");
    if (fp2 == NULL) {
        printf("Error opening file output.log for writing\n");
        exit(-1);
    }

   
   
    while (fgets(line, BUFSIZ, fp1) != NULL) {
        strcpy(buf, line); // line we read read into the buffer
        createarray(line, args); 
        
        time(&t1); // starting time
        pid = fork(); // we'll fork a new process

        if (pid == 0) { // 0 means child process
            // filenames for stdout and stderr redirection
            char outFile[BUFSIZ], errFile[BUFSIZ];
            snprintf(outFile, sizeof(outFile), "%d.out", getpid()); // output filename
            snprintf(errFile, sizeof(errFile), "%d.err", getpid()); // error filename

            // open the output and error and exits upon failing
            FILE *out = fopen(outFile, "w");
            FILE *err = fopen(errFile, "w");
            if (out == NULL || err == NULL) {
                perror("Error opening output or error file");
                exit(EXIT_FAILURE); 
            }

            // dup2 to redirect our file to std our or error https://man7.org/linux/man-pages/man2/dup.2.html
            dup2(fileno(out), STDOUT_FILENO);  
            dup2(fileno(err), STDERR_FILENO);  

            // the file pointers are no longer needed in the child so close 
            fclose(out);
            fclose(err);



            execvp(args[0], args); // child process is replaced with the command
            perror("exec"); // if the exec fails
            exit(-1); // we exit
        } else if (pid > 0) { // if it is the parent process
            printf("Child started at %s", ctime(&t1)); // log when the child starts
            printf("Wait for the child process to terminate\n");
            wait(&status); // wait for child to termingate
            time(&t2); // end ing time
            printf("Child ended at %s", ctime(&t2)); // log the time

            // check if the child ended like it should have
            if (WIFEXITED(status)) { 
                printf("Child process exited with status = %d\n", WEXITSTATUS(status)); // Log exit status
            } else { 
                printf("Child process did not terminate normally!\n"); // Log abnormal termination
            }

            // log command execution time
            buf[strlen(buf) - 1] = '\t'; // replace \n with \t
            strcat(buf, ctime(&t1)); // write the start time to command log
            buf[strlen(buf) - 1] = '\t'; // replace \n added by ctime with \t
            strcat(buf, ctime(&t2)); // append to the end time
            fprintf(fp2, "%s", buf); // write log to output.log
            fflush(fp2); \
        } else { // we get an error with the fork
            perror("fork"); 
            exit(EXIT_FAILURE); // exit on fork failure
        }
    }

    fclose(fp1);
    fclose(fp2);
    printf("[%ld]: Exiting main program .....\n", (long)getpid()); 
    
    return 0; 
}
