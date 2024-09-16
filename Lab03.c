#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void displayArray(char **arr, int size);
void sortArray(char **arr, int size);
void readStrings(char **arr, int size);

void displayArray(char **arr, int size) {
    printf("[\n");
    for (int i = 0; i < size; i++) {
        printf("  %s\n", arr[i]);
    }
    printf("]\n");
}

void sortArray(char **arr, int size) {
    for (int i = 1; i < size; i++) {
        char *key = arr[i];
        int j = i - 1;
        while (j >= 0 && strcmp(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void readStrings(char **arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("Enter string %d: ", i + 1);
        arr[i] = (char *)malloc(100 * sizeof(char));  // Allocate memory for each string
        if (arr[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        scanf("%s", arr[i]);
    }
}

int main() {
    int N;
    printf("Enter the number of strings: ");
    scanf("%d", &N);

    // Allocate memory for array of string pointers
    char **arr = (char **)malloc(N * sizeof(char *));
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Read strings into the array
    readStrings(arr, N);

    // Sort the array
    sortArray(arr, N);

    // Display the sorted array
    printf("Sorted array:\n");
    displayArray(arr, N);

    // Free allocated memory
    for (int i = 0; i < N; i++) {
        free(arr[i]);  // Free each string
    }
    free(arr);  // Free array of pointers

    return 0;
}