#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[])//I kept the parameters the same as our hello world on Thursday.
{
int n;

 printf("Enter the number of elements in the array: ");
    scanf("%d", &n);

//declare the array with specified size 
int arr[n];

 // Read array elements from user
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

 int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        // Move elements of arr[0..i-1] that are greater than key to one position ahead
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }

 int i;
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");


 printf("Sorted array:\n");


return;

}

