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




int main(){










}