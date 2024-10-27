#include <stdio.h>      // This is for using print functions
#include <stdlib.h>     // This is for using functions like exit()
#include <string.h>     // This is for working with strings
#include <dirent.h>     // This is for reading directories
#include <sys/stat.h>   // This is for getting info about files
#include <unistd.h>     // This is for using functions for system stuff
#include <sys/types.h>  // This is for types used in system calls
#include <wait.h>       // This is for waiting for processes to finish

// This function counts how many words are in a file
int count_words(const char *filename) {
    FILE *file = fopen(filename, "r"); // Open the file to read
    if (!file) { // Check if opening the file worked
        perror("Unable to open file"); // Print an error message if it didn't work
        return -1; // Return -1 to say there was a problem
    }

    int word_count = 0; // Start with zero words
    char buffer[1024]; // Create a space to hold each word we read

    // Keep reading words until there are none left
    while (fscanf(file, "%1023s", buffer) == 1) {
        word_count++; // Count each word we find
    }

    fclose(file); // Close the file when done
    return word_count; // Return how many words we counted
}

// This is where the program starts running
int main(int argc, char *argv[]) {
    // Check if we got the right number of arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]); // Tell the user how to use the program
        exit(EXIT_FAILURE); // Exit because we did not get the right input
    }

    const char *directory_name = argv[1]; // Get the name of the directory from the input
    DIR *dir = opendir(directory_name); // Try to open the directory
    if (!dir) { // Check if opening the directory worked
        perror("Error opening directory"); // Print an error message if it didn't work
        exit(EXIT_FAILURE); // Exit because we couldn't open the directory
    }

    struct dirent *entry; // This will hold each item we find in the directory
    struct stat file_stat; // This will hold info about each file

    // Read each item in the directory one by one
    while ((entry = readdir(dir)) != NULL) { // Keep going until there are no more items
        // Make a full path to the item we are looking at
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", directory_name, entry->d_name);

        // Check if the item is a regular file
        if (stat(file_path, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
            pid_t pid = fork(); // Make a new child process
            if (pid < 0) { // Check if making the child failed
                perror("Fork failed"); // Print an error message
                closedir(dir); // Close the directory before we leave
                exit(EXIT_FAILURE); // Exit because we had a problem
            }

            if (pid == 0) { // This part runs in the child process
                // Count how many words are in the file
                int words = count_words(file_path);
                if (words == -1) exit(EXIT_FAILURE); // If counting failed, just exit

                // Print out the details of the file: name, size, and word count
                printf("File: %s | Size: %ld bytes | Words: %d\n", entry->d_name, file_stat.st_size, words);
                exit(EXIT_SUCCESS); // Child process is done and exits
            }
        }
    }

    closedir(dir); // Close the directory when we're done looking
    while (wait(NULL) > 0); // Wait for all child processes to finish

    return 0; // Exit the program successfully
}