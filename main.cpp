// main.cpp
#include <unistd.h>
#include "auth.h"
#include "routes.h"
#include "global.h"

int api_call_count = 0;

// Function to display the ETAs with colors
void display_etas_with_colors(const char* station_uid, EtaInfo* etas, int eta_count) {
    printf("\n=== ETAs @ Station %s ===\n", station_uid);
    for (int i = 0; i < eta_count; i++) {
        int mins = etas[i].estimate_time / 60;

        // Apply color based on the remaining time
        if (mins <= 1) {
            // Red for buses arriving soon
            printf("\033[1m%-18s\033[0m (%s): \033[1;31m%2d min\033[0m\n", etas[i].route_name, etas[i].route_uid, mins);
        } else if (mins <= 5) {
            // Yellow for buses coming in 1-5 minutes
            printf("\033[1m%-18s\033[0m (%s): \033[1;33m%2d min\033[0m\n", etas[i].route_name, etas[i].route_uid, mins);
        } else {
            // Green for buses arriving in more than 5 minutes
            printf("\033[1m%-18s\033[0m (%s): \033[1;32m%2d min\033[0m\n", etas[i].route_name, etas[i].route_uid, mins);
        }
    }
}

int main() {
    char CITY[20] = {};
    char STATION_UID[20] = {}; 
    char token[2048] = {0};

    if (get_oauth_token(token, sizeof(token)) != 0) {
        printf("❌ Auth failed\n");
        return 1;
    }

    printf("Enter City Name:");
    scanf("%s", CITY);

    printf("Enter Station UID:");
    scanf("%s", STATION_UID);

    
    StationStop stops[MAX_STOPS];
    EtaInfo     etas[MAX_ETAS];

    int stop_count = fetch_station_stops(CITY,STATION_UID, token, stops, MAX_STOPS);
    if (stop_count < 1) {
        printf("❌ No stops for StationUID %s\n", STATION_UID);
        return 1;
    }

    while (1) {
        int eta_count = fetch_etas(CITY, stops, stop_count, token, etas, MAX_ETAS);
        if (eta_count < 0) {
            printf("❌ Fetch ETA failed\n");
            break;
        }
        // Display the number of API calls made so far
        printf("[DEBUG] Total API calls made: %d\n", api_call_count);
        
        // Call the function to display ETAs with color
        display_etas_with_colors(STATION_UID, etas, eta_count);

        printf("\n⏱ Refreshing in 60s...\n");
        sleep(60);
    }


    return 0;
}
