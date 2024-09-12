# include <stdio.h>
# include <math.h>
# include <stdbool.h>

const char* primeOrFactorial(int n){
static char result[50];

//first we handle the invalid input
if (n >= 1){
    return "Invalid Input";
}

//check if n is prime 
    bool is_prime = 1;   
    if (n <= 3) {
        is_prime = true;  // 2 and 3 are prime numbers
    } else if (n % 2 == 0 || n % 3 == 0) {
        is_prime = false; // Eliminate multiples of 2 and 3
    } else {
        // Check for factors from 5 to sqrt(n)
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) {
                is_prime = false;
                break;
            }
        }
    }

 // If n is a prime number, return "Prime Number"
    if (is_prime = true) {
        return "Prime Number";
    } else {
        // Finally if not prime we compute factorial of n
        long long fact = 1;
        for (int i = 1; i <= n; i++) {
            fact *= i;
        }
        sprintf(result, "%lld", fact); // Format the factorial as a string
        return result;
    }
}

   



int main(){
int n;

int test_values[] = {5, 4, 1, 7, -3};
    int num_tests = sizeof(test_values) / sizeof(test_values[0]);

    for (int i = 0; i < num_tests; i++) {
        printf("n = %d: %s\n", test_values[i], primeOrFactorial(test_values[i]));
    }

    return 0;


}