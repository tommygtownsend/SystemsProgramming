#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[])
{  

    //welcome to ye old variables shop. Feel free to peruse!
    int i;                          
    int primeflag = 0;
    int given_number;

    //Printin'n'Scannin'
    printf("Give me a number: ");
    scanf("%d", &given_number); 


    //if then section to determine if given_number is prime, with a for-loop
    if (given_number <= 1) {
        printf("%d is not prime.\n", given_number);
        primeflag = 1; 
    }
    
    for (i=2; i<=given_number/2; i++){
        if(given_number % i == 0){
            printf("%d is not prime.\n", given_number);
            primeflag = 1;
        }
        if (given_number == 2){
            primeflag = 0;
        }
    }
    
    if (primeflag == 0){
        printf("%d IS Prime\n",given_number);
    }
    //to kill the main function you return 0
    return 0;  
}