#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LINESIZE 1024
#define MAX_ITEMS 1000

struct listing {
    int id;
    int host_id;
    char *host_name;
    char *neighbourhood_group;
    char *neighbourhood;
    float latitude;
    float longitude;
    char *room_type;
    float price;
    int minimum_nights;
    int number_of_reviews;
    int calculated_host_listings_count;
    int availability_365;
};




struct listing getfields(char* line) {
    struct listing item;
    char *token;

    token = strtok(line, ",");
    item.id = atoi(token);
    
    token = strtok(NULL, ",");
    item.host_id = atoi(token);
    
    token = strtok(NULL, ",");
    item.host_name = strdup(token);
    
    token = strtok(NULL, ",");
    item.neighbourhood_group = strdup(token);
    
    token = strtok(NULL, ",");
    item.neighbourhood = strdup(token);
    
    token = strtok(NULL, ",");
    item.latitude = atof(token);
    
    token = strtok(NULL, ",");
    item.longitude = atof(token);
    
    token = strtok(NULL, ",");
    item.room_type = strdup(token);
    
    token = strtok(NULL, ",");
    item.price = atof(token);
    
    token = strtok(NULL, ",");
    item.minimum_nights = atoi(token);
    
    token = strtok(NULL, ",");
    item.number_of_reviews = atoi(token);
    
    token = strtok(NULL, ",");
    item.calculated_host_listings_count = atoi(token);
    
    token = strtok(NULL, ",");
    item.availability_365 = atoi(token);

    return item;
}

void displayStruct(struct listing item) {
    printf("ID: %d, Host ID: %d, Host Name: %s, Neighbourhood Group: %s, "
           "Neighbourhood: %s, Latitude: %.6f, Longitude: %.6f, Room Type: %s, "
           "Price: %.2f, Minimum Nights: %d, Number of Reviews: %d, "
           "Calculated Host Listings Count: %d, Availability 365: %d\n",
           item.id, item.host_id, item.host_name, item.neighbourhood_group,
           item.neighbourhood, item.latitude, item.longitude, item.room_type,
           item.price, item.minimum_nights, item.number_of_reviews,
           item.calculated_host_listings_count, item.availability_365);
}


int compareByHostName(const void *a, const void *b) {
    return strcmp(((struct listing*)a)->host_name, ((struct listing*)b)->host_name);
}

int compareByPrice(const void *a, const void *b){
    float priceA = ((struct listing*)a)->price;
    float priceB = ((struct listing*)b)->price;
    return (priceA > priceB) - (priceA < priceB);
}

void writeToFile(const char *filename, struct listing *list_items, int count) {
    FILE *fptr = fopen(filename, "w");
    if (!fptr) {
        perror("Error opening file");
        return;
    }
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
    fclose(fptr);
}

int main(){

     FILE *fptr;
    char line[LINESIZE];
    struct listing list_items[MAX_ITEMS];
    int count = 0;

    fptr = fopen("listings.csv", "r");
    if (!fptr) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while (fgets(line, LINESIZE, fptr) != NULL) {
        list_items[count++] = getfields(line);
    }
    fclose(fptr);

    printf("Data read from listings.csv:\n");
    for (int i = 0; i < count; i++) {
        displayStruct(list_items[i]);
    }

    // Sort by host_name
    qsort(list_items, count, sizeof(struct listing), compareByHostName);
    writeToFile("sorted_by_host_name.csv", list_items, count);

    // Sort by price
    qsort(list_items, count, sizeof(struct listing), compareByPrice);
    writeToFile("sorted_by_price.csv", list_items, count);

    // Free allocated memory
    for (int i = 0; i < count; i++) {
        free(list_items[i].host_name);
        free(list_items[i].neighbourhood_group);
        free(list_items[i].neighbourhood);
        free(list_items[i].room_type);
    }

    return 0;
}