//To compile: "gcc -o Lab05ttownsen Lab05ttownsen.c"
//To run: "./Lab05ttownsen ."


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>//https://manpages.ubuntu.com/manpages/trusty/man7/string.h.7posix.html
#include <sys/stat.h>//https://man7.org/linux/man-pages/man0/sys_stat.h.0p.html

void travdir(const char *dirname, int *count) {//returns nothing, takes in a string for the directory name and a pointer to an integer representing the number of files printed
    struct dirent *dirent;//pointer to a structure which represents the directory we will read later
    DIR *parentDir = opendir(dirname);//attempts to open the directory dirname if it works will point to the paerant directory. if it falis it will set to null

    if (parentDir == NULL) {//If the parent directory is null
        printf("Error opening directory '%s'\n", dirname);//error message that the dir was null
        return;//exits
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
            
            if (S_ISDIR(path_stat.st_mode)) {//S_ISDIR check if directory.
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
    travdir(argv[1], &count);//again, travdir takes in the directory specified when running the program, as well as the mem location for the file counter
    return 0;
}
//this one was very difficult to implement! I am reading a lot more files in the directory than I expected. Upon more reading these extra files are a lot of data git stores to keep up with the edits, metadata, pushes, pulls, compiles, ect. All hidden  
//staging commits then rerunning yeilded more files in my list
//I hope it was ok to include all these extra files, as i could not figure out how to not include them. I should have started way earlier but life got in the way as it sometime does.