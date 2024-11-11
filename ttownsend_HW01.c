ls#include <stdio.h>
#include <stdbool.h> //is it ok that I included bool header?
#include <math.h>
//*******************************************
//*******************************************
//*******************************************

// Function to check if a number is prime and compute factorial if not prime
const char* primeOrFactorial(int n) {//Takes in an in and returns a immutable pointer to an array of characters
    
    static char result[50]; // Buffer to hold the result string
    
    // Handle invalid input
    if (n <= 1) {
        return "Invalid Input"; //Same as in Lab01
    }

    // Check if n is a prime number
    bool is_prime = true;//using bool looks better to me than my primeFlag implementation in Lab01
    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0) {
            is_prime = false;
            break;//go ahead and leave if we flip the is_prime
        }
    }

    if (is_prime) {
        return "Prime Number";
    } else {
        // Compute factorial of n
        long long fact = 1;  //I want really big numbers, and long long does 64 bit https://www.w3schools.com/java/ref_keyword_long.asp
        //important: https://www.youtube.com/watch?v=6-1Ue0FFrHY *************

        for (int i = 1; i <= n; i++) {
            fact *= i;
        }
        sprintf(result, "%lld", fact); // Format the factorial with the long specifier as a string
        //https://www.w3schools.com/c/ref_stdio_sprintf.php
        return result;
    }
}
//*******************************************
//*******************************************
//*******************************************
//*******************************************
//*******************************************
//*******************************************

// Function to compute the sum of elements equal to their indices
int UABIndexSum(int arr[], int size) {
    int sum = 0; // Initialize sum to 0
    for (int i = 0; i < size; i++) {
        if (arr[i] == i) {
            sum += arr[i]; // Add to sum if element matches its index
        }
    }
    return sum; // Return the result
}
//*******************************************
//*******************************************
//*******************************************

//fuction that goes through an array and replaces evens with zero
void replaceEvenWithZero(int arr[], int size) {
    // First go through the whole array
    for (int i = 0; i < size; i++) {
        // Check if the current element is even
        if (arr[i] % 2 == 0) {
            // Replace even number with 0
            arr[i] = 0;
        }
    }
}

//*******************************************
//*******************************************
//*******************************************

bool evenSquare(int n) {
    if (n < 0) {
        return false; // Negative numbers can't be squares of real numbers
    }
    // Calculate the integer square root of n
    int root = (int)sqrt(n); //I can't just include the math library for this one, had to compile with the addition -lm flag at he end
    // Check if n is a perfect square
    if (root * root != n) {
        return false;
    }
    // Check if the square root is even
    return (root % 2 == 0);
}

//*******************************************
//*******************************************
//*******************************************

// Function to count the number of vowels in a string
int countVowels(const char *s) {
    int count = 0;
    // Iterate through each character of the string
    while (*s) {
        char ch = *s; // Get the current character
        // Check if the character is a vowel (considering both uppercase and lowercase)
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
            ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U') {
            count++;
        }
        s++; // Move to the next character
    }    
    return count;
}

//*******************************************
//*******************************************
//*******************************************

int main() {


printf("******************************************************************\n");
// Tester for primeOrFactorial()    
    int test_values[] = {5, 4, 1, 7, -3, 16, 8};
    int num_tests = sizeof(test_values) / sizeof(test_values[0]);
    for (int i = 0; i < num_tests; i++) {
        printf("n = %d: %s\n", test_values[i], primeOrFactorial(test_values[i]));
    }


printf("******************************************************************\n");
// Tester for UABIndexSum()
    int arr1[] = {0, 2, 2, 4};
    int arr2[] = {3, 1, 0, 4};
    int arr3[] = {1, 2, 3, 3};
    printf("Sum for arr1: %d\n", UABIndexSum(arr1, sizeof(arr1) / sizeof(arr1[0])));
    printf("Sum for arr2: %d\n", UABIndexSum(arr2, sizeof(arr2) / sizeof(arr2[0])));
    printf("Sum for arr3: %d\n", UABIndexSum(arr3, sizeof(arr3) / sizeof(arr3[0])));


printf("******************************************************************\n");
// Tester for replaceEvenWithZero()
    int q3arr1[] = {1, 2, 3, 4};
    int size1 = sizeof(q3arr1) / sizeof(q3arr1[0]);
    replaceEvenWithZero(q3arr1, size1);
    printf("Output: [");
    for (int i = 0; i < size1; i++) {
        printf("%d", q3arr1[i]);
        if (i < size1 - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    int q3arr2[] = {2, 4, 6};
    int size2 = sizeof(q3arr2) / sizeof(q3arr2[0]);
    replaceEvenWithZero(q3arr2, size2);
    printf("Output: [");
    for (int i = 0; i < size2; i++) {
        printf("%d", q3arr2[i]);
        if (i < size2 - 1) {
            printf(", ");
        }
    }
    printf("]\n");


printf("******************************************************************\n");
// Tester for evenSquare()
    int testValues[] = {16, 15, 25, 36};
    int numTests = sizeof(testValues) / sizeof(testValues[0]);

    for (int i = 0; i < numTests; i++) {
        int n = testValues[i];
        if (evenSquare(n)) {
            printf("%d is an even square\n", n);
        } else {
            printf("%d is not an even square\n", n);
        }
    }


printf("******************************************************************\n");
// Tester for countVowels()
    const char *test1 = "Hello World";
    const char *test2 = "UAB CS";
    const char *test3 = "Python";
    const char *test4 = "aeiou";
    
    printf("Number of vowels in '%s': %d\n", test1, countVowels(test1));
    printf("Number of vowels in '%s': %d\n", test2, countVowels(test2));
    printf("Number of vowels in '%s': %d\n", test3, countVowels(test3));
    printf("Number of vowels in '%s': %d\n", test4, countVowels(test4));

   return 0; 
}