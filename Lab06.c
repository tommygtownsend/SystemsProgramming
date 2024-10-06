#include <stdio.h>   // Includes standard input/output functions
#include <stdlib.h>  // Includes functions for memory allocation, conversion, etc.
#include <string.h>  // Includes string manipulation functions

// Constants for line size and maximum number of items
#define LINESIZE 1024
#define MAX_ITEMS 1000

// Define a structure called 'listing' to hold the data for each item
struct listing {
    int id;  // Unique identifier for the listing
    int host_id;  // Identifier for the host of the listing
    char *host_name;  // Name of the host (dynamically allocated string)
    char *neighbourhood_group;  // Group of the neighbourhood (dynamically allocated string)
    char *neighbourhood;  // Name of the neighbourhood (dynamically allocated string)
    float latitude;  // Latitude of the listing
    float longitude;  // Longitude of the listing
    char *room_type;  // Type of room (dynamically allocated string)
    float price;  // Price of the listing
    int minimum_nights;  // Minimum number of nights required to book
    int number_of_reviews;  // Total number of reviews for the listing
    int calculated_host_listings_count;  // Total listings by the host
    int availability_365;  // Number of days available in a year
};

// Function to parse a line of CSV data into a 'listing' structure
struct listing getfields(char* line) {
    struct listing item;  // Create an instance of 'listing'
    char *token;  // Pointer to hold the tokens while parsing

    // Use strtok to split the line by commas
    token = strtok(line, ",");
    item.id = atoi(token);  // Convert token to integer and assign to id
    
    token = strtok(NULL, ",");
    item.host_id = atoi(token);  // Convert token to integer and assign to host_id
    
    token = strtok(NULL, ",");
    item.host_name = strdup(token);  // Duplicate token string and assign to host_name
    
    token = strtok(NULL, ",");
    item.neighbourhood_group = strdup(token);  // Duplicate token string for neighbourhood_group
    
    token = strtok(NULL, ",");
    item.neighbourhood = strdup(token);  // Duplicate token string for neighbourhood
    
    token = strtok(NULL, ",");
    item.latitude = atof(token);  // Convert token to float and assign to latitude
    
    token = strtok(NULL, ",");
    item.longitude = atof(token);  // Convert token to float and assign to longitude
    
    token = strtok(NULL, ",");
    item.room_type = strdup(token);  // Duplicate token string for room_type
    
    token = strtok(NULL, ",");
    item.price = atof(token);  // Convert token to float and assign to price
    
    token = strtok(NULL, ",");
    item.minimum_nights = atoi(token);  // Convert token to integer for minimum_nights
    
    token = strtok(NULL, ",");
    item.number_of_reviews = atoi(token);  // Convert token to integer for number_of_reviews
    
    token = strtok(NULL, ",");
    item.calculated_host_listings_count = atoi(token);  // Convert token to integer
    
    token = strtok(NULL, ",");
    item.availability_365 = atoi(token);  // Convert token to integer for availability_365

    return item;  // Return the populated structure
}

// Function to display the contents of a 'listing' structure
void displayStruct(struct listing item) {
    // Print all the fields of the 'listing' structure in a formatted way
    printf("ID: %d, Host ID: %d, Host Name: %s, Neighbourhood Group: %s, "
           "Neighbourhood: %s, Latitude: %.6f, Longitude: %.6f, Room Type: %s, "
           "Price: %.2f, Minimum Nights: %d, Number of Reviews: %d, "
           "Calculated Host Listings Count: %d, Availability 365: %d\n",
           item.id, item.host_id, item.host_name, item.neighbourhood_group,
           item.neighbourhood, item.latitude, item.longitude, item.room_type,
           item.price, item.minimum_nights, item.number_of_reviews,
           item.calculated_host_listings_count, item.availability_365);
}

// Comparison function for sorting by host name (used by qsort)
int compareByHostName(const void *a, const void *b) {
    return strcmp(((struct listing*)a)->host_name, ((struct listing*)b)->host_name);
}

// Comparison function for sorting by price (used by qsort)
int compareByPrice(const void *a, const void *b) {
    float priceA = ((struct listing*)a)->price;  // Get price of the first item
    float priceB = ((struct listing*)b)->price;  // Get price of the second item
    // Return -1, 0, or 1 based on the comparison of prices
    return (priceA > priceB) - (priceA < priceB);
}

// Function to write the listings to a new CSV file
void writeToFile(const char *filename, struct listing *list_items, int count) {
    FILE *fptr = fopen(filename, "w");  // Open file for writing
    if (!fptr) {  // Check if the file opened successfully
        perror("Error opening file");
        return;  // Exit the function if there is an error
    }
    // Loop through each listing and write to the file
    for (int i = 0; i < count; i++) {
        fprintf(fptr, "%d,%d,%s,%s,%s,%.6f,%.6f,%s,%.2f,%d,%d,%d,%d\n",
                list_items[i].id, list_items[i].host_id, list_items[i].host_name,
                list_items[i].neighbourhood_group, list_items[i].neighbourhood,
                list_items[i].latitude, list_items[i].longitude,
                list_items[i].room_type, list_items[i].price,
                list_items[i].minimum_nights, list_items[i].number_of_reviews,
                list_items[i].calculated_host_listings_count,
                list_items[i].availability_365);
    }
    fclose(fptr);  // Close the file after writing
}

// Main function - entry point of the program
int main() {
    FILE *fptr;  // File pointer for reading the input file
    char line[LINESIZE];  // Buffer to hold each line read from the file
    struct listing list_items[MAX_ITEMS];  // Array to hold the listings
    int count = 0;  // Counter for the number of items read

    fptr = fopen("listings.csv", "r");  // Open the CSV file for reading
    if (!fptr) {  // Check if the file opened successfully
        perror("Error opening file");
        return EXIT_FAILURE;  // Exit the program if there is an error
    }

    // Read each line from the file until the end
    while (fgets(line, LINESIZE, fptr) != NULL) {
        // Check if we are exceeding the maximum number of items
        if (count >= MAX_ITEMS) {
            fprintf(stderr, "Exceeded maximum items limit.\n");
            break;  // Stop reading if we hit the limit
        }
        list_items[count++] = getfields(line);  // Parse the line and store the result
    }
    fclose(fptr);  // Close the file after reading

    // Display all the read data
    printf("Data read from listings.csv:\n");
    for (int i = 0; i < count; i++) {
        displayStruct(list_items[i]);  // Call the display function for each item
    }

    // Sort the listings by host_name
    qsort(list_items, count, sizeof(struct listing), compareByHostName);
    writeToFile("sorted_by_host_name.csv", list_items, count);  // Write sorted data to file

    // Sort the listings by price
    qsort(list_items, count, sizeof(struct listing), compareByPrice);
    writeToFile("sorted_by_price.csv", list_items, count);  // Write sorted data to file

    // Free allocated memory for dynamic strings in the structure
    for (int i = 0; i < count; i++) {
        free(list_items[i].host_name);  // Free host_name
        free(list_items[i].neighbourhood_group);  // Free neighbourhood_group
        free(list_items[i].neighbourhood);  // Free neighbourhood
        free(list_items[i].room_type);  // Free room_type
    }

    return 0;  // Return success
}