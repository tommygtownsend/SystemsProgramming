/*
Name: Your Name
BlazerId: Your Blazer ID
Project #: Your Project Number
To compile: gcc -o traverse traverse.c -Wall
To run: ./traverse [options] [directory]
Options:
-S           Show file sizes
-s <size>    Show files larger than or equal to <size> bytes
-f <pattern> Show files matching <pattern>
-r           Reverse sort by file size
*/

#include <stdio.h>       // printf, perror
#include <stdlib.h>      // atoi, malloc, free
#include <string.h>      // strdup, strstr
#include <dirent.h>      // opendir, readdir, closedir
#include <sys/stat.h>    // lstat, struct stat
#include <unistd.h>      // S_ISDIR, readlink
#include <getopt.h>      // getopt
#include <fcntl.h>       // open

#define MAX_FILES 1024   // Maximum number of files to handle

// Structure to hold file information
typedef struct {
    char *name;         // File name
    off_t size;        // File size
} FileEntry;

// Function to print file information, including size if required
void print_file_info(const char *file_name, struct stat *file_stat, int show_size) {
    if (show_size) {
        printf("%s (%ld bytes)", file_name, file_stat->st_size);
    } else {
        printf("%s", file_name);
    }
}

// Comparison function for sorting files by size
int compare_files(const void *a, const void *b) {
    FileEntry *fileA = (FileEntry *)a; // Cast to FileEntry pointer
    FileEntry *fileB = (FileEntry *)b; // Cast to FileEntry pointer
    return (fileA->size - fileB->size); // Compare sizes
}

// Recursive function to traverse directories
void traverse(const char *dir_name, int depth, int show_size, off_t min_size, const char *filter, int reverse_sort) {
    DIR *dir = opendir(dir_name); // Open directory
    if (!dir) {
        perror("opendir"); // Print error if unable to open
        return;
    }

    struct dirent *entry; // Directory entry
    struct stat file_stat; // To hold file status
    char path[1024];      // Buffer for file path

    // Array to hold files for sorting
    FileEntry files[MAX_FILES];
    int file_count = 0;

    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue; // Skip hidden files

        // Create the full path for the current file
        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);
        lstat(path, &file_stat); // Get file statistics

        // Check if it matches the filter
        if (filter && strstr(entry->d_name, filter) == NULL) continue;

        // Check size filtering
        if (file_stat.st_size < min_size) continue;

        // Add file info to the array
        files[file_count].name = strdup(entry->d_name); // Duplicate the file name
        files[file_count].size = file_stat.st_size; // Store the file size
        file_count++;
    }

    closedir(dir); // Close the directory

    // Sort files by size
    qsort(files, file_count, sizeof(FileEntry), compare_files);
    if (reverse_sort) {
        for (int i = 0; i < file_count / 2; i++) {
            FileEntry temp = files[i]; // Temporary swap variable
            files[i] = files[file_count - 1 - i]; // Swap with the reverse index
            files[file_count - 1 - i] = temp; // Complete the swap
        }
    }

    // Print the directory name with indentation based on depth
    printf("%*s%s:\n", depth * 4, "", dir_name);
    for (int i = 0; i < file_count; i++) {
        snprintf(path, sizeof(path), "%s/%s", dir_name, files[i].name); // Full path for file
        lstat(path, &file_stat); // Get file statistics
        printf("%*s", (depth + 1) * 4, ""); // Indentation for files
        print_file_info(files[i].name, &file_stat, show_size); // Print file info
        printf("\n");

        // Recursively traverse directories
        if (S_ISDIR(file_stat.st_mode)) { // Check if it's a directory
            traverse(path, depth + 1, show_size, min_size, filter, reverse_sort); // Recursive call
        }

        free(files[i].name); // Free the duplicated file name
    }
}

int main(int argc, char **argv) {
    int show_size = 0, reverse_sort = 0; // Flags for options
    off_t min_size = 0; // Minimum size filter
    const char *filter = NULL; // Pattern filter
    char *start_dir = "."; // Default to current directory

    int opt;
    // Process command-line options
    while ((opt = getopt(argc, argv, "Ss:f:r:")) != -1) {
        switch (opt) {
            case 'S':
                show_size = 1; // Set flag to show sizes
                break;
            case 's':
                min_size = atoi(optarg); // Get minimum size
                break;
            case 'f':
                filter = optarg; // Get filter pattern
                break;
            case 'r':
                reverse_sort = 1; // Set flag for reverse sort
                break;
            default:
                fprintf(stderr, "Usage: %s [-S] [-s size] [-f pattern] [-r] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Get starting directory from arguments
    if (optind < argc) {
        start_dir = argv[optind]; // Get directory if provided
    }

    traverse(start_dir, 0, show_size, min_size, filter, reverse_sort); // Start traversal
    return 0;
}
