/*
Name: Tommy Townsend
BlazerId: ttonwsend
Project: HW02
To compile: gcc -o traverse traverse.c
To run: ./traverse [options] [directory]
*/
//I may have been a little overzealous with comments and man7 links. Going througth the things I do know,  
//and going through the documentation, helps with learning the more difficult things availablbe to us in c I think
#include <stdio.h>      // For printf and perror
#include <stdlib.h>     // For malloc, free, atoi
#include <string.h>     // For strdup and strstr
#include <dirent.h>     // For opendir, readdir, closedir
#include <sys/stat.h>   // For lstat and struct stat
#include <getopt.h>     // For getopt
#include <fcntl.h>      // file control stuff, wouldnt let me use the off_t without


#define MAX_FILES 1024  //Do we need to allow for more files? 

typedef struct {        //Struct for our name and off_t for the size of the file
    char *name;
    off_t size; 
} FileEntry;


// Function to print information about a file
// Parameters:
// - file_name: The name of the file to print
// - file_stat: A pointer to the struct that holds the file's statistics (size, etc.)
// - show_size: Flag to determine whether to display the size of the file
// - depth: The current depth in the directory tree for indentation
void print_file_info(const char *file_name, struct stat *file_stat, int show_size, int depth) {
    if (show_size) {
        printf("%*s%s (%ld bytes)\n", depth * 4, "", file_name, file_stat->st_size);// If show_size is set, print the file name and its size
    } else {
        printf("%*s%s\n", depth * 4, "", file_name);//If not just print the file name
    }
}

// Comparison function for sorting files by size
// This function is used by qsort to determine the order of files
// Parameters:
// - a: Pointer to the first file entry
// - b: Pointer to the second file entry
// Returns: Negative value if a < b, zero if a == b, positive value if a > b
int compare_files(const void *a, const void *b) {
    FileEntry *fileA = (FileEntry *)a;
    FileEntry *fileB = (FileEntry *)b;
    return (fileA->size - fileB->size);
}


// Function to recursively traverse directories
// Parameters:
// - dir_name: name of the directory we will traverse
// - depth: Current depth level in the directory hierarchy (used for indentation)
// - show_size: flag to indicate whether to show file sizes
// - min_size: mim file size to display (files smaller than this are skipped)
// - filter: Optional string to filter file names (only files containing this string will be shown)
// - reverse_sort: Flag to indicate reversal
void traverse(const char *dir_name, int depth, int show_size, off_t min_size, const char *filter, int reverse_sort) {
    DIR *dir = opendir(dir_name); // Open the directory (https://man7.org/linux/man-pages/man3/opendir.3.html)
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry; // Directory entry (https://man7.org/linux/man-pages/man struct dirent.7.html)
    struct stat file_stat; // File status structure (https://man7.org/linux/man-pages/man2/stat.2.html)
    char path[1024]; // Buffer for full path

    // array to sort them later if needed
    FileEntry files[MAX_FILES];
    int file_count = 0;

    while ((entry = readdir(dir)) != NULL) { // Read directory entries (https://man7.org/linux/man-pages/man3/readdir.3.html)
        if (entry->d_name[0] == '.') continue; // Skip hidden files, this one allowed me to get rid of TONS of files I didnt want to see

        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name); // create our full path (https://man7.org/linux/man-pages/man3/snprintf.3.html)
        lstat(path, &file_stat); // Get file statuses when we need (https://man7.org/linux/man-pages/man2/lstat.2.html)

        // Check if it matches the filter
        if (filter && strstr(entry->d_name, filter) == NULL) continue; // Check for the enetered substring (https://man7.org/linux/man-pages/man3/strstr.3.html)

        // Check size filtering
        if (file_stat.st_size < min_size) continue;

        // Add to files array
        files[file_count].name = strdup(entry->d_name); // Duplicate the file name (https://man7.org/linux/man-pages/man3/strdup.3.html)
        files[file_count].size = file_stat.st_size;
        file_count++;
    }

    closedir(dir); // Close the directory (https://man7.org/linux/man-pages/man3/closedir.3.html)

    // Sort files
    qsort(files, file_count, sizeof(FileEntry), compare_files); // Sort the files (https://man7.org/linux/man-pages/man3/qsort.3.html)
    if (reverse_sort) {
        for (int i = 0; i < file_count / 2; i++) {
            FileEntry temp = files[i];
            files[i] = files[file_count - 1 - i];
            files[file_count - 1 - i] = temp;
        }
    }//https://www.youtube.com/watch?v=aXXWXz5rF64

    // Print directory name with indentation
    printf("%*s%s:\n", depth * 4, "", dir_name); // Indentation based on depth

    for (int i = 0; i < file_count; i++) {
        snprintf(path, sizeof(path), "%s/%s", dir_name, files[i].name);
        lstat(path, &file_stat); // Get file status

        // Print file info with indentation
        print_file_info(files[i].name, &file_stat, show_size, depth + 1);

        // Recursively traverse directories
        if (S_ISDIR(file_stat.st_mode)) { // Check if the file is a directory (https://man7.org/linux/man-pages/man3/S_ISDIR.3.html)
            traverse(path, depth + 1, show_size, min_size, filter, reverse_sort);
        }

        free(files[i].name); // Free allocated memory for file name (https://man7.org/linux/man-pages/man3/free.3.html)
    }
}

int main(int argc, char **argv) {
    int show_size = 0, reverse_sort = 0;
    off_t min_size = 0;
    const char *filter = NULL;
    char *start_dir = "."; // Default to current directory

    int opt;
    while ((opt = getopt(argc, argv, "Ss:f:r")) != -1) { // Get command-line options (https://man7.org/linux/man-pages/man3/getopt.3.html)
        switch (opt) {
            case 'S':
                show_size = 1;
                break;
            case 's':
                min_size = atoi(optarg); // Convert argument to integer (https://man7.org/linux/man-pages/man3/atoi.3.html)
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
