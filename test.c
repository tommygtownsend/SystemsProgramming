#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

// Function to count words in a file
int count_words(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return -1; // Return -1 on failure
    }

    int word_count = 0;
    char buffer[1024];

    while (fscanf(file, "%1023s", buffer) == 1) {
        word_count++;
    }

    fclose(file);
    return word_count;
}

// Main function
int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *directory_name = argv[1];
    DIR *dir = opendir(directory_name); // Open the directory
    if (!dir) {
        perror("Error opening directory");
        exit(EXIT_FAILURE); // Exit if the directory cannot be opened
    }

    struct dirent *entry; // Structure to hold directory entries
    while ((entry = readdir(dir)) != NULL) { // Read entries
        if (entry->d_type == DT_REG) { // Only consider regular files
            pid_t pid = fork(); // Create a child process
            if (pid < 0) {
                perror("Fork failed");
                closedir(dir);
                exit(EXIT_FAILURE); // Exit if fork fails
            }

            if (pid == 0) { // Child process
                char file_path[1024];
                snprintf(file_path, sizeof(file_path), "%s/%s", directory_name, entry->d_name); // Construct full file path

                // Get file size using stat
                struct stat file_stat;
                if (stat(file_path, &file_stat) == -1) {
                    perror("Error getting file stats");
                    exit(EXIT_FAILURE);
                }

                // Get word count
                int words = count_words(file_path);
                if (words == -1) exit(EXIT_FAILURE; // Handle file read error

                // Print file details
                printf("File: %s | Size: %ld bytes | Words: %d\n", entry->d_name, file_stat.st_size, words);
                exit(EXIT_SUCCESS); // Child process exits after completing its task
            }
        }
    }

    closedir(dir); // Close the directory
    while (wait(NULL) > 0); // Parent process waits for all children to finish

    return 0; // Exit successfully
}
