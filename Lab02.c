#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[])//I kept the parameters the same as our hello world on our first lab.
{
int n;

printf("Enter the number of elements in the array: ");
scanf("%d", &n);//taking in as int, storing in n

//declare the array with specified size 
int arr[n]; //the array gets n values, each having 4 bytes

 // Read array elements from user
    printf("Enter %d elements:\n", n); //print(thing you're printing with variable specifier, variable)
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]); //scanning as int, storing in the current memory location of the arr
    }


//start of insertion sort 
 int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i]; //key is the current value of array
        j = i - 1; //j is the index before i

        // Move elements of arr[0..i-1] that are greater than key to one position ahead
        while (j >= 0 && arr[j] > key) { //while j not negative and the key value is smaller than the value in j
            arr[j + 1] = arr[j]; // replaces the array value at j with the array value at j+1
            j = j - 1; //decrements j
        }
        arr[j + 1] = key; //new key value
    }

//finally we print the sorted array
 printf("Sorted array:\n");
 int i2;
    for (i2 = 0; i2 < n; i2++) {
        printf("Index# [%d] :",i2);
        printf(" %d ", arr[i2]);
        printf("\n");
    }
    //printf("\n");

return 0; //return 0 to kill the program

}

