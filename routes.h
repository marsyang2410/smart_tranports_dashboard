// routes.h
#ifndef ROUTES_H
#define ROUTES_H

#define MAX_STOPS   32
#define MAX_ETAS    64

struct StationStop{
    char stop_uid[32];
    char route_uid[32];
    char route_name[64];
};

struct EtaInfo{
    char stop_uid[32];
    char route_uid[32];
    char route_name[64];
    int  estimate_time;
};

int fetch_station_stops(const char* city,
                        const char* station_uid,
                        const char* token,
                        StationStop* stops,
                        int max_stops);

int fetch_etas(const char* city,
                const StationStop* stops,
                int stop_count,
                const char* token,
                EtaInfo* etas,
                int max_etas);

#endif // ROUTES_H
