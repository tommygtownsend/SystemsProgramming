#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void traverse_directory(const char *dirname, int *count) {
    struct dirent *dirent;
    DIR *parentDir = opendir(dirname);

    if (parentDir == NULL) {
        printf("Error opening directory '%s'\n", dirname);
        return;
    }

    while ((dirent = readdir(parentDir)) != NULL) {
        // Skip the special entries "." and ".."
        if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
            // Print the current directory/file name
            printf("[%d] %s/%s\n", (*count)++, dirname, dirent->d_name);
            
            // Construct the full path
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dirname, dirent->d_name);

            // Check if the entry is a directory
            struct stat path_stat;
            stat(path, &path_stat);
            if (S_ISDIR(path_stat.st_mode)) {
                // Recursively traverse the subdirectory
                traverse_directory(path, count);
            }
        }
    }

    closedir(parentDir);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <dirname>\n", argv[0]);
        exit(-1);
    }

    int count = 1;
    traverse_directory(argv[1], &count);
    return 0;
}