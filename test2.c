#include <stdio.h>      // for our basic functions
#include <stdlib.h>     // functions like exit()
#include <string.h>     // strings
#include <dirent.h>     // reading directories
#include <sys/stat.h>   // for getting info about files
#include <unistd.h>     // for system stuff
#include <sys/types.h>  // this is for types used in system calls
#include <wait.h>       // for waiting for processes to finish




//This function counts how many words are in a file
//Function: count_words 
//In: pointer to a constant character, the filename
//Out: an integer, -1 if it fails or the word count if it is successful
int count_words(const char *filename) {
    FILE *file = fopen(filename, "r"); // opens file we will read
    if (!file) { // if opening the file worked
        perror("Unable to open file"); // print an error message if it didn't work
        return -1; // Return -1 to say there was a problem
    }

    int word_count = 0; // no words at start
    char buffer[1024]; // space to hold each word we read

    // read the words until there are none left
    while (fscanf(file, "%1023s", buffer) == 1) {
        word_count++; // increment the count
    }

    fclose(file); // close it
    return word_count; // Return how many words we counted
}



int main(int argc, char *argv[]) {

    if (argc != 2) {      // if we have the right amount of argument
        fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]); // little message to tell user what it needs
        exit(EXIT_FAILURE); // exit the whole thing
    }

    const char *directory_name = argv[1]; //name of the directory comes from the input
    DIR *dir = opendir(directory_name); // try and open the directory
    if (!dir) { // if opening the directory happened
        perror("Error opening directory"); // give an erroe message
        exit(EXIT_FAILURE); // exit
    }

//Big Dan's Struct Barn Get Your Fine Structs Here!
    struct dirent *entry; // this will hold each item we find in the directory
    struct stat file_stat; // holds our file stats

    
    while ((entry= readdir(dir)) != NULL) { // keep going until there are no more items to read
        // we need a full path to the item we are looking at
        char file_path[1024]; //make enough space!
        snprintf(file_path, sizeof(file_path), "%s/%s", directory_name, entry->d_name);

     
        if (stat(file_path, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {   // Check if the item is a regular file
            pid_t pid = fork(); // Bring a beautiful new baby child process into the world.
            if (pid < 0) { // negative means it failed
                perror("Fork failed"); // Print error message
                closedir(dir); // Close the directory before we leave, always good for the memory
                exit(EXIT_FAILURE); // exit the program
            }


            if (pid == 0) { // if its a child process
                // Count how many words are in the file
                int words = count_words(file_path);//the counnt words function we made
                if (words == -1) exit(EXIT_FAILURE); // if counting failed, just exit

                // Print out the details of the file: name, size, and word count
                printf("File: %s | Size: %ld bytes | Words: %d\n", entry->d_name, file_stat.st_size, words);
                exit(EXIT_SUCCESS); // our child is finished and exits
            }
        }
    }

    closedir(dir); // be nice to our memory!
    while (wait(NULL) > 0); // Wait for all child processes to finish

    return 0; // exit the whole thing
}