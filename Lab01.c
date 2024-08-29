#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[])
{
    int given_number;
    printf("Gimmie a number!");
    scanf("%d",&given_number);
    if (given_number <= 1)
    printf("%d is prime");
     
    return 0;  

}
