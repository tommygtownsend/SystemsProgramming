
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Mutex to make it so the value can only be accessed one at a time 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// struct with the necessary data
typedef struct {
    double *a;     // Pointer to the array of numbers
    double sum;    // Partial sum computed by this thread
    int N;         // Total number of elements
    int size;      // Number of threads
    long tid;      // Thread ID
} ThreadData;

// computes the sum of a portion of the array
void *compute(void *arg) {
    ThreadData *data = (ThreadData *)arg;  // cast argument to ThreadData
    int myStart, myEnd, myN, i;

    myN = data->N / data->size;
    myStart = data->tid * myN;
    myEnd = myStart + myN;
    if (data->tid == (data->size - 1)) {
        myEnd = data->N;  // last thread will take care of left over
    }

    // Compute the partial sum for this thread's portion of the array
    double mysum = 0.0;
    for (i = myStart; i < myEnd; i++) {
        mysum += data->a[i];
    }

    // Lock the mutex to update the global sum safely
    pthread_mutex_lock(&mutex);
    data->sum += mysum;
    pthread_mutex_unlock(&mutex);

    return (NULL);
}

int main(int argc, char **argv) {
    long i;
    pthread_t *tid;
    ThreadData *threadData;  // Array to store data for each thread

    if (argc != 3) {
        printf("Usage: %s <# of elements> <# of threads>\n", argv[0]);
        exit(-1);
    }

    int N = atoi(argv[1]);  // Number of elements
    int size = atoi(argv[2]);  // Number of threads

    tid = (pthread_t *)malloc(sizeof(pthread_t) * size);
    threadData = (ThreadData *)malloc(sizeof(ThreadData) * size);

    double *a = (double *)malloc(sizeof(double) * N);
    for (i = 0; i < N; i++) {
        a[i] = (double)(i + 1);
    }

    // each thread gets its own data
    for (i = 0; i < size; i++) {
        threadData[i].a = a;
        threadData[i].N = N;
        threadData[i].size = size;
        threadData[i].tid = i;  // thread ID
        threadData[i].sum = 0.0;  // Initialize sum for this thread

        // Create the thread and pass the thread data to it
        pthread_create(&tid[i], NULL, compute, (void *)&threadData[i]);
    }

    // Wait for threads to finish
    for (i = 0; i < size; i++) {
        pthread_join(tid[i], NULL);
    }

    // sum for everything
    double totalSum = 0.0;
    for (i = 0; i < size; i++) {
        totalSum += threadData[i].sum;  // Aggregate partial sums from each thread
    }

    printf("The total sum is %g, it should be equal to %g\n",
           totalSum, ((double)N * (N + 1)) / 2);

    // be nice to your memory and it will be nice to you
    free(tid);
    free(threadData);
    free(a);
    return 0;
}
