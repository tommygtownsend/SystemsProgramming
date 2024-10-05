/*
Name: Your Name
BlazerId: Your Blazer ID
Project #: Your Project Number
To compile: gcc -o test test.c
To run: ./test <options> <directory>
*/

#include <stdio.h>   // For printf and perror functions
#include <stdlib.h>  // For memory allocation and exit functions
#include <dirent.h>  // For directory operations (like reading directories)
#include <string.h>  // For string operations (like comparison)
#include <sys/stat.h> // For checking file types (like regular files and directories)
#include <unistd.h>  // For getopt function to parse command-line options
#include <fcntl.h>

// Structure to hold file information
typedef struct {
    char *name;      // Name of the file or directory
    off_t size;     // Size of the file
    int is_dir;      // 1 if it's a directory, 0 if it's a file
    char *symlink_target; // Target if it's a symlink
} FileEntry;

// Global variables to hold entries
FileEntry *file_entries = NULL; // Array of file entries
size_t file_count = 0;           // Count of files

// Function to traverse directories
void travdir(const char *dirname) {
    struct dirent *dirent;           // To read directory entries
    DIR *parentDir = opendir(dirname); // Open the directory

    // If the directory can't be opened, print an error and return
    if (parentDir == NULL) {
        perror("Error opening directory");
        return;
    }

    // Read each entry in the directory
    while ((dirent = readdir(parentDir)) != NULL) {
        // Skip the special entries "." and ".."
        if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
            char path[1024]; // Buffer to hold the full path
            snprintf(path, sizeof(path), "%s/%s", dirname, dirent->d_name); // Create full path

            struct stat path_stat; // Structure to hold file info
            if (lstat(path, &path_stat) == -1) { // Get file info
                perror("Error getting file status");
                continue; // Skip this entry if there's an error
            }

            // Allocate memory for a new file entry
            file_entries = realloc(file_entries, sizeof(FileEntry) * (file_count + 1));
            // Store file information
            file_entries[file_count].name = strdup(dirent->d_name); // Copy file name
            file_entries[file_count].size = path_stat.st_size; // Get size
            file_entries[file_count].is_dir = S_ISDIR(path_stat.st_mode); // Check if it's a directory
            file_entries[file_count].symlink_target = NULL; // No symlink target initially
            file_count++; // Increment the count

            // If it's a directory, traverse it
            if (file_entries[file_count - 1].is_dir) {
                travdir(path); // Call the function recursively for directories
            }
        }
    }
    closedir(parentDir); // Close the directory
}

// Main function
int main(int argc, char **argv) {
    // If no directory is specified, use the current directory
    const char *dirname = (argc > 1) ? argv[1] : "."; 

    // Start traversing the directory
    travdir(dirname);

    // Print the collected file entries
    for (size_t i = 0; i < file_count; i++) {
        printf("%s%s", file_entries[i].name, file_entries[i].is_dir ? " (directory)" : ""); // Print name
        if (file_entries[i].size > 0) { // If it's a file with size, print size
            printf(" (%ld bytes)", file_entries[i].size);
        }
        printf("\n"); // New line for next entry
        free(file_entries[i].name); // Free allocated memory for name
    }
    free(file_entries); // Free the file entries array
    return EXIT_SUCCESS; // Return success status
}
