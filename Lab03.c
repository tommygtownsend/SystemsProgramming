#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   

// display the array of strings
void displayArray(char **arr, int size) {
    printf("[\n");  // Print opening bracket for the array display
    for (int i = 0; i < size; i++) {  // Iterate over each string in the array
        printf("  %s\n", arr[i]);  // Print each string with indentation
    }
    printf("]\n");  
}

// sort the array of strings using insertion sort like in lab 01
void sortArray(char **arr, int size) {
    for (int i = 1; i < size; i++) {  // Start from the second element
        char *key = arr[i];  // Store the current string we are looking at to be inserted
        int j = i - 1;  // Set j to the INDEX before the current element
        while (j >= 0 && strcmp(arr[j], key) > 0) {  // Compare and shift elements
        //https://www.w3schools.com/c/ref_string_strcmp.php#:~:text=Definition%20and%20Usage,a%20string%20has%20been%20reached.
            arr[j + 1] = arr[j];  // Move string arr[j] to the next position
            j--;  // Move left in the array
        }
        arr[j + 1] = key;  // Insert the key at its correct position
        //the values are swapped this way
    }
}

// Function to read strings from the user and  then store them in the array
void readStrings(char **arr, int size) {
    for (int i = 0; i < size; i++) {  // Iterate over the number of strings
        printf("Enter string %d: ", i + 1);  // Prompt user to enter each string
        arr[i] = (char *)malloc(100 * sizeof(char));  // Allocate memory for each string
        if (arr[i] == NULL) {  // Check if memory allocation failed
            fprintf(stderr, "Memory allocation failed\n");  // Print error message to stderr
            exit(1);  // Exit the program with an error code
        }
        scanf("%s", arr[i]);  // Read the string from user input
    }
}

int main() {
    int N;  // Variable to store the number of strings
    printf("Enter the number of strings: ");  
    scanf("%d", &N);  // Int, N, Read the number of strings from user input

    // Allocate memory for an array of string pointers
    // I am still not entirely comfortable with malloc, and have a lot of questions on the pointers for thursday
    char **arr = (char **)malloc(N * sizeof(char *));//I'm allocating the size of a single character in my array times the total number of elements I want
    if (arr == NULL) {  // Check if memory allocation failed
        fprintf(stderr, "Memory allocation failed\n");  // Print error message to stderr https://stackoverflow.com/questions/3385201/confused-about-stdin-stdout-and-stderr
        //used when handling errors !
        return 1;  // Return an error code
    }

    // Read strings
    readStrings(arr, N);

    // Then Sort the array of strings
    sortArray(arr, N);

    // Finally Display the sorted array of strings
    printf("Sorted array:\n");
    displayArray(arr, N);

    // We should be in the habit of freeing up our memory, else we could get a memory leak (exaple being any game made in unity)
    for (int i = 0; i < N; i++) {
        free(arr[i]);  // Free the memory allocated for each string
    }
    free(arr);  // Free the memory allocated for the array of string pointers

    return 0;  // Return 0 to exit
}
