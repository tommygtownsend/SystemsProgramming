#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFSIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "Error: The two filenames must be different.\n");
        exit(EXIT_FAILURE);
    }

    int readFileDescriptor = open(argv[1], O_RDONLY);
    if (readFileDescriptor == -1) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    int writeFileDescriptor = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0700);
    if (writeFileDescriptor == -1) {
        perror("Error opening output file");
        close(readFileDescriptor);
        exit(EXIT_FAILURE);
    }

    char buf[BUFFSIZE];
    ssize_t n;
    while ((n = read(readFileDescriptor, buf, BUFFSIZE)) > 0) {
        if (write(writeFileDescriptor, buf, n) != n) {
            fprintf(stderr, "Write error\n");
            close(readFileDescriptor);
            close(writeFileDescriptor);
            exit(EXIT_FAILURE);
        }
    }

    if (n == -1) {
        perror("Read error");
    }

    close(readFileDescriptor);
    close(writeFileDescriptor);
    printf("Contents of %s have been copied to %s successfully.\n", argv[1], argv[2]);
    return EXIT_SUCCESS;
}