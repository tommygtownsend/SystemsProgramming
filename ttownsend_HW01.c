# include <stdio.h>
# include <math.h>
# include <stdbool.h>

const char* primeOrFactorial(int n){
static char result[50];

//first we handle the invalid input
if (n <= 1){
    return "Invalid Input";
}

for (int i=2; i<=n/2; i++){
    if (n % i == 0){
        //return " Not Prime Number";
        int fact = 1;
        for (int i = 1; i <= n; i++) {
            fact *= i;
        }
        sprintf(result, "%lld", fact); // Format the factorial as a string
        return result;
    }
}


int main(){
int n;

int test_values[] = {5, 4, 1, 7, -3, 16, 8};
    int num_tests = sizeof(test_values) / sizeof(test_values[0]);

    for (int i = 0; i < num_tests; i++) {
        n=test_values[i];
        printf("n = %d: %s\n", test_values[i], primeOrFactorial(test_values[i]));
    }

    return 0;


}