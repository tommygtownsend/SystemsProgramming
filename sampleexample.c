#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void travdir(const char *dirname, int *count) {//returns nothing, takes in a string for the directory name and a pointer to an integer representing the number of files printed
    struct dirent *dirent;//pointer to a dirent structure which represents the deirectory we will read later
    DIR *parentDir = opendir(dirname);//attempts to open the directory dirname if it works will point to the paerant directory. if it falis it will set to null

    if (parentDir == NULL) {//If the parent directory is null
        printf("Error opening directory '%s'\n", dirname);//error message that the dir was null
        return;
    }

    while ((dirent = readdir(parentDir)) != NULL) {//If there are directories to read..
        // Skip the special entries "." and ".."
        if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {//should skip directories starting with . and ..
            // Print the current directory/file name
            printf("[%d] %s/%s\n", (*count)++, dirname, dirent->d_name);//increases the count pointer to go past them
            //We must skip these directories or we will get infinite recursion!


            // Construct the full path
            char path[1024];//we need to set aside some space
            snprintf(path, sizeof(path), "%s/%s", dirname, dirent->d_name);//we create a character array path to store the full path of the current entry. 
            //snprintf formats the string, combining dirname and dirent->d_name into path
            //https://joequery.me/code/snprintf-c/

            // Check if the entry is a directory
            struct stat path_stat;//a struct called path_stat 
            stat(path, &path_stat);//takes info from a file at path and stores in stat_path
            if (S_ISDIR(path_stat.st_mode)) {//S_ISDIR check if directory. If it is, then 
                // Recursively traverse the subdirectory
                travdir(path, count);//If its a directroy we recursivley call travdir again!
            }
        }
    }
    closedir(parentDir);//we must close the directory to free up its resouces!
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <dirname>\n", argv[0]);//we need two arguments when we run, the program call and a directory to recursively traverse
        exit(-1);
    }

    int count = 1;
    travdir(argv[1], &count);
    return 0;
}
