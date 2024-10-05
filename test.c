#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
    char *name;
    off_t size;
    int is_dir;
} FileEntry;

FileEntry *file_entries = NULL;
size_t file_count = 0;
char *filter = NULL; // To store the filter string

void travdir(const char *dirname) {
    struct dirent *dirent;
    DIR *parentDir = opendir(dirname);
    if (parentDir == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((dirent = readdir(parentDir)) != NULL) {
        if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
            if (filter != NULL && strstr(dirent->d_name, filter) == NULL) {
                continue; // Skip if the name does not contain the filter
            }

            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dirname, dirent->d_name);
            struct stat path_stat;
            if (lstat(path, &path_stat) == -1) {
                perror("Error getting file status");
                continue;
            }

            file_entries = realloc(file_entries, sizeof(FileEntry) * (file_count + 1));
            file_entries[file_count].name = strdup(dirent->d_name);
            file_entries[file_count].size = path_stat.st_size;
            file_entries[file_count].is_dir = S_ISDIR(path_stat.st_mode);
            file_count++;

            if (file_entries[file_count - 1].is_dir) {
                travdir(path);
            }
        }
    }
    closedir(parentDir);
}

int main(int argc, char **argv) {
    // Command-line argument parsing
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                filter = argv[i + 1]; // Set the filter string
                i++; // Skip the next argument
            }
        } else {
            // Assume it's a directory
            const char *dirname = argv[i];
            travdir(dirname);
        }
    }

    // Output collected file entries
    for (size_t i = 0; i < file_count; i++) {
        printf("%s (%ld bytes)\n", file_entries[i].name, file_entries[i].size);
        free(file_entries[i].name);
    }
    free(file_entries);
    return EXIT_SUCCESS;
}
