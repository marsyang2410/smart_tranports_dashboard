// main.cpp
#include <unistd.h>
#include "auth.h"
#include "routes.h"

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

        printf("\n=== ETAs @ Station %s ===\n", STATION_UID);
        for (int i = 0; i < eta_count; i++) {
            int mins = etas[i].estimate_time / 60;
            printf("%-6s (%s): %2d min\n",
                   etas[i].route_uid,
                   etas[i].route_name,
                   mins);
        }

        printf("\n⏱ Refreshing in 60s...\n");
        sleep(60);
    }

    return 0;
}
