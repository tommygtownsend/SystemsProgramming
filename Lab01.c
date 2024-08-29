#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[])
{  
    int i;
    int primeflag = 0;
    int given_number;
    printf("Give me a number: ");
    scanf("%d", &given_number);

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
    return 0;  
}