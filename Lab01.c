#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[])//I kept the paramters the same as our hello world on Thursday.

{  
    int a = 1;
    while (a){//like in fridays lecture, I used the good ole infinite while loop to keep the program running 

    //welcome to ye old variables shop. Feel free to peruse!
    int i;                          
    int primeflag = 0;//in the court of law, all ints are assumed prime unless proven otherwise
    int given_number;

    //Printin'n'Scannin'
    printf("Give me a number: ");
    scanf("%d", &given_number); 

    //logical section
    if (given_number <= 1) {
        primeflag = 1; 
    }
    //we check if any of the first half of possible divisors yeild no remainder. If so, raise the flag
    for (i=2; i<=given_number/2; i++){
        if(given_number % i == 0){
            primeflag = 1;
        }
        if (given_number == 2){//2 is only prime divisible by 2 
            primeflag = 0;
        }
    }
    //I used a switch case for the answer, like in the friday lecture. I like how clean it looks!
    switch (primeflag) {
        case 1:
            printf("%d is NOT prime.\n", given_number);
        break;
        case 0: 
            printf("%d IS Prime\n",given_number);
        break;
    }
    }
    //to kill the main function you return 0
    return 0;//we'll never make it to this line, because of the while loop.      
}