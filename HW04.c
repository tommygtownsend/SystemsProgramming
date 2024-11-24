#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

// Constants
#define NUM_RANDOM_NUMBERS 500    // Numbers per thread
#define NUM_THREADS 3             // Number of threads created by the parent
#define NUM_CHILD_THREADS 10      // Number of threads in the child process
#define MAX_RANDOM_NUMBER 1000    // Maximum number for random generation
#define PIPE_READ 0
#define PIPE_WRITE 1

// Mutex for synchronizing access to the shared sum variable
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Pipe setup for communication between threads/processes
int pipefd[2];

// Structure to store thread data
typedef struct {
    int *pipefd; // pipe file descriptors
    int tid;     // Thread ID
} ThreadData;

// Thread function to generate random numbers and send them via pipe
void *generate_random_numbers(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int *pipefd = data->pipefd;
    int tid = data->tid;
    int numbers[NUM_RANDOM_NUMBERS];

    // Generate random numbers
    for (int i = 0; i < NUM_RANDOM_NUMBERS; i++) {
        numbers[i] = rand() % (MAX_RANDOM_NUMBER + 1);
    }

    // Write numbers to pipe
    for (int i = 0; i < NUM_RANDOM_NUMBERS; i++) {
        if (write(pipefd[PIPE_WRITE], &numbers[i], sizeof(int)) == -1) {
            perror("Write to pipe failed");
            exit(EXIT_FAILURE);
        }
    }

    printf("Thread %d sent %d random numbers to the pipe.\n", tid, NUM_RANDOM_NUMBERS);
    return NULL;
}

// Child thread function to read from the pipe and compute the sum
void *compute_sum(void *arg) {
    int *pipefd = ((ThreadData *)arg)->pipefd;
    int numbers[150];  // 150 numbers per thread
    int sum = 0;

    // Read 150 numbers from the pipe and compute the sum
    for (int i = 0; i < 150; i++) {
        if (read(pipefd[PIPE_READ], &numbers[i], sizeof(int)) == -1) {
            perror("Read from pipe failed");
            exit(EXIT_FAILURE);
        }
        sum += numbers[i];
    }

    // Lock the mutex to safely update the shared sum
    pthread_mutex_lock(&mutex);
    ((ThreadData *)arg)->tid = sum; // Store sum in thread's data
    pthread_mutex_unlock(&mutex);

    printf("Child thread computed sum: %d\n", sum);
    return NULL;
}

int main() {
    srand(time(NULL)); // Seed random number generator

    if (pipe(pipefd) == -1) {  // Create pipe
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    // Parent process creates threads to generate random numbers
    pthread_t parent_threads[NUM_THREADS];
    ThreadData parent_thread_data[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        parent_thread_data[i].pipefd = pipefd;
        parent_thread_data[i].tid = i;
        pthread_create(&parent_threads[i], NULL, generate_random_numbers, &parent_thread_data[i]);
    }

    // Wait for all parent threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(parent_threads[i], NULL);
    }

    // Close write end of the pipe (parent won't write anymore)
    close(pipefd[PIPE_WRITE]);

    // Create child process to compute sums
    pid_t pid = fork();
    if (pid == 0) {  // Child process
        pthread_t child_threads[NUM_CHILD_THREADS];
        ThreadData child_thread_data[NUM_CHILD_THREADS];
        int total_sum = 0;

        // Create threads in child process to compute sum
        for (int i = 0; i < NUM_CHILD_THREADS; i++) {
            child_thread_data[i].pipefd = pipefd;
            pthread_create(&child_threads[i], NULL, compute_sum, &child_thread_data[i]);
        }

        // Wait for all child threads to finish
        for (int i = 0; i < NUM_CHILD_THREADS; i++) {
            pthread_join(child_threads[i], NULL);
            total_sum += child_thread_data[i].tid;  // Add up all thread sums
        }

        // Calculate average
        double average = total_sum / (double)NUM_CHILD_THREADS;
        printf("Child process: Average sum is %.2f\n", average);

        // Write result to file
        FILE *output_file = fopen("output.txt", "w");
        if (output_file) {
            fprintf(output_file, "Average sum: %.2f\n", average);
            fclose(output_file);
        } else {
            perror("Error opening output file");
        }

        exit(EXIT_SUCCESS);
    } else if (pid > 0) {  // Parent process
        // Wait for the child process to complete
        wait(NULL);
    } else {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // Clean up and close pipe
    close(pipefd[PIPE_READ]);
    return 0;
}
