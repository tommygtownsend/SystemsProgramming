/*
Name: Tommy Townsend
BlazerId: ttonwsend
Project: HW02
To compile: gcc -o traverse traverse.c
To run: ./traverse [options] [directory]
*/

#include <stdio.h>      // For printf and perror
#include <stdlib.h>     // For malloc, free, atoi
#include <string.h>     // For strdup and strstr
#include <dirent.h>     // For opendir, readdir, closedir
#include <sys/stat.h>   // For lstat and struct stat
#include <getopt.h>     // For getopt
#include <fcntl.h>

#define MAX_FILES 1024

typedef struct {
    char *name;
    off_t size; 
} FileEntry;

// Function to print file information
void print_file_info(const char *file_name, struct stat *file_stat, int show_size) {
    if (show_size) {
        printf("%s (%ld bytes)", file_name, file_stat->st_size);
    } else {
        printf("%s", file_name);
    }
}

// Comparison function for sorting files by size
int compare_files(const void *a, const void *b) {
    FileEntry *fileA = (FileEntry *)a;
    FileEntry *fileB = (FileEntry *)b;
    return (fileA->size - fileB->size);
}

// Function to recursively traverse directories
void traverse(const char *dir_name, int depth, int show_size, off_t min_size, const char *filter, int reverse_sort) {
    DIR *dir = opendir(dir_name); // Open the directory
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    struct stat file_stat;
    char path[1024];

    // Collect files in an array to sort them later if needed
    FileEntry files[MAX_FILES];
    int file_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue; // Skip hidden files

        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name); // Create full path
        lstat(path, &file_stat); // Get file status

        // Check if it matches the filter
        if (filter && strstr(entry->d_name, filter) == NULL) continue; // Check for substring

        // Check size filtering
        if (file_stat.st_size < min_size) continue;

        // Add to files array
        files[file_count].name = strdup(entry->d_name); // Duplicate the file name
        files[file_count].size = file_stat.st_size;
        file_count++;
    }

    closedir(dir); // Close the directory

    // Sort files
    qsort(files, file_count, sizeof(FileEntry), compare_files);
    if (reverse_sort) {
        for (int i = 0; i < file_count / 2; i++) {
            FileEntry temp = files[i];
            files[i] = files[file_count - 1 - i];
            files[file_count - 1 - i] = temp;
        }
    }

    // Print directory name with indentation
    printf("%*s%s:\n", depth * 4, "", dir_name); // Indentation based on depth

    for (int i = 0; i < file_count; i++) {
        snprintf(path, sizeof(path), "%s/%s", dir_name, files[i].name);
        lstat(path, &file_stat); // Get file status
        print_file_info(files[i].name, &file_stat, show_size);
        printf("\n");

        // Recursively traverse directories
        if (S_ISDIR(file_stat.st_mode)) {
            traverse(path, depth + 1, show_size, min_size, filter, reverse_sort);
        }

        free(files[i].name); // Free allocated memory for file name
    }
}

int main(int argc, char **argv) {
    int show_size = 0, reverse_sort = 0;
    off_t min_size = 0;
    const char *filter = NULL;
    char *start_dir = "."; // Default to current directory

    int opt;
    while ((opt = getopt(argc, argv, "Ss:f:r:")) != -1) {
        switch (opt) {
            case 'S':
                show_size = 1;
                break;
            case 's':
                min_size = atoi(optarg);
                break;
            case 'f':
                filter = optarg;
                break;
            case 'r':
                reverse_sort = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-S] [-s size] [-f pattern] [-r] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        start_dir = argv[optind]; // Get directory if provided
    }

    traverse(start_dir, 0, show_size, min_size, filter, reverse_sort);
    return 0;
}