/*This one was harder than the others for me to wrap my head around. 
The open() command makes more sense, and I'm starting to get more comfortable
with always thinking of my data as just a stream of characters!

To compile: "gcc -o Lab04 Lab04.c"
To run: "./Lab04 file1.txt file2.txt"
To open file(s): "cat file1.txt"

*/

#include <stdio.h>//our standard input and out put library
#include <stdlib.h>//we need this library for exit 
#include <unistd.h>//needed for read, write, close
#include <fcntl.h>//file coperation for open

#define BUFFSIZE 4096 //buffer size for reading and writing

int main(int argc, char *argv[]) {
int readFileDescriptor, writeFileDescriptor; //file desc. for our input and output files
long int n;//how many bytes are read
char buf[BUFFSIZE];//character array which is the size of our buffer to to hold data read from the file

//3 arguments must be provided
if (argc != 3){
    printf("Usage: %s <source_filename> <destination_filename>\n",argv[0]);
   exit (-1);//exit with an error code
}
//open source in read only mode
readFileDescriptor = open(argv[2], O_RDONLY);
writeFileDescriptor = open(argv[1], O_CREAT|O_WRONLY| O_APPEND, 0700); //create, write only, and append mode. 
//Vaishak showed me that you can open argv[2] in read, then use that data to o_append it. "https://man7.org/linux/man-pages/man2/open.2.html"

//checking if file opening was successful
if (readFileDescriptor == -1 || writeFileDescriptor == -1){
    printf("Error with file open\n");//error if either one doesn't open
    exit (-1);//exit with the error code
}

//loop through until there are no more bytes to read
while ((n = read(readFileDescriptor, buf, BUFFSIZE)) > 0){
    //write the bytes into the destination file
    //is the number of bytes written equal the number of bytes read?
    if (write(writeFileDescriptor, buf, n) != n){
        //error if it doesnt write
        printf("Error writing to output file\n");
        exit (-1);
    }
}

//check if there is an error reading the input file
if (n < 0){
    printf("Error reading input file\n");
    exit (-1);
}

//be sure to close them both at the end!
close(readFileDescriptor);
close(writeFileDescriptor);
return 0;//end the program
}
