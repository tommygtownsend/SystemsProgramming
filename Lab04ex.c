#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFSIZE 4096


int main(int argc, char *argv[]) {
int readFileDescriptor, writeFileDescriptor;
long int n;
char buf[BUFFSIZE];

if (argc != 3){
printf("Usage: %s <source_filename> <destination_filename>\n",
argv[0]);
exit (-1);
}
readFileDescriptor = open(argv[1], O_RDONLY);
writeFileDescriptor = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0700);

/*
Use the open function in read only mode to read the input file. The open function
takes the name of the file as the first argument and the open flag as the second
argument. The open flag specifies if the file should be opened in read only mode
(O_RDONLY), write only mode (O_WRONLY), or read-write mode (O_RDWR). There is
an optional third argument that specifies the file permissions called mode, we will not
use the optional third argument here. The third argument specifies the file permissions
of the new file created for writing. Note that the UNIX file uses {read, write, execute}
(rwx) permissions for the user, group, and everyone. You can find out various modes
supported by the open function from the man page or the text book.
*/
if (readFileDescriptor == -1 || writeFileDescriptor == -1){
printf("Error with file open\n");
exit (-1);
}

while ((n = read(readFileDescriptor, buf, BUFFSIZE)) > 0){
if (write(writeFileDescriptor, buf, n) != n){
printf("Error writing to output file\n");
exit (-1);
}
}
if (n < 0){
printf("Error reading input file\n");
exit (-1);
}


close(readFileDescriptor);
close(writeFileDescriptor);
return 0;
}
