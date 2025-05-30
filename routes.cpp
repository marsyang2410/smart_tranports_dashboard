// routes.cpp
#include "routes.h"
#include "http.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>

//static const char* CITY = "Taipei";

int fetch_station_stops(const char* city,
                        const char* station_uid,
                        const char* token,
                        StationStop* stops,
                        int max_stops)
{
    char url[512] = {0};
    snprintf(url, sizeof(url),
      "https://tdx.transportdata.tw/api/basic/v2/Bus/Station/City/%s"
      "?%%24select=Stops&%%24filter=StationUID%%20eq%%20%%27%s%%27&%%24format=JSON",
      city, station_uid);

    char resp[8192] = {0};
    if (http_get(url, token, resp, sizeof(resp)) != 0)
        return -1;

    printf("[DEBUG] Station response: %.200s…\n", resp);

    cJSON* root = cJSON_Parse(resp);
    if (!root) return -1;

    cJSON* first = cJSON_GetArrayItem(root, 0);
    cJSON* stops_array = cJSON_GetObjectItem(first, "Stops");
    int count = cJSON_GetArraySize(stops_array);
    int n = 0;
    for (int i = 0; i < count && n < max_stops; i++) {
        cJSON* item = cJSON_GetArrayItem(stops_array, i);
        cJSON* su = cJSON_GetObjectItem(item, "StopUID");
        cJSON* sn = cJSON_GetObjectItem(cJSON_GetObjectItem(item, "StopName"), "Zh_tw");
        cJSON* ru = cJSON_GetObjectItem(item, "RouteUID");
        cJSON* rn = cJSON_GetObjectItem(cJSON_GetObjectItem(item, "RouteName"), "En");
        if (su && sn && ru && rn) {
            strncpy(stops[n].stop_uid,   su->valuestring, sizeof(stops[n].stop_uid)-1);
            strncpy(stops[n].stop_name,   sn->valuestring, sizeof(stops[n].stop_name)-1);
            strncpy(stops[n].route_uid,  ru->valuestring, sizeof(stops[n].route_uid)-1);
            strncpy(stops[n].route_name, rn->valuestring, sizeof(stops[n].route_name)-1);
            n++;
        }
    }
    cJSON_Delete(root);
    printf("[DEBUG] Found %d stops for station %s\n", n, station_uid);
    return n;
}

int fetch_etas( const char* city,
                const StationStop* stops,
                int stop_count,
                const char* token,
                EtaInfo* etas,
                int max_etas)
{
    // Build filter
    char filter[1024] = {0};
    // start with encoded OData params:
    strcat(filter,
        "?%24format=JSON"
        "&%24select=StopUID,RouteUID,EstimateTime"
        "&%24filter=");
    
    // for each stop, OR‑chain with proper encoding
    for (int i = 0; i < stop_count; i++) {
        if (i) strcat(filter, "%20or%20");
        strcat(filter, "StopUID%20eq%20%27");
        strcat(filter, stops[i].stop_uid);
        strcat(filter, "%27");
    }
  

    char url[1536] = {0};
    snprintf(url, sizeof(url),
      "https://tdx.transportdata.tw/api/basic/v2/Bus/EstimatedTimeOfArrival/City/%s%s",
      city, filter);

    char resp[8192] = {0};
    if (http_get(url, token, resp, sizeof(resp)) != 0)
        return -1;

    printf("[DEBUG] ETA response: %.200s…\n", resp);

    cJSON* root = cJSON_Parse(resp);
    if (!root) return -1;
    int count = cJSON_GetArraySize(root), n = 0;

    for (int i = 0; i < count && n < max_etas; i++) {
        cJSON* it = cJSON_GetArrayItem(root, i);
        cJSON* su = cJSON_GetObjectItem(it, "StopUID");
        cJSON* ru = cJSON_GetObjectItem(it, "RouteUID");
        cJSON* et = cJSON_GetObjectItem(it, "EstimateTime");
        cJSON* ss = cJSON_GetObjectItem(it, "StopStatus");
        if (su && ru && ss) {
            // find name
            const char* name = "";
            for (int j = 0; j < stop_count; j++) {
                if (strcmp(stops[j].stop_uid, su->valuestring)==0 &&
                    strcmp(stops[j].route_uid, ru->valuestring)==0)
                {
                    name = stops[j].route_name;
                    break;
                }
            }
            strncpy(etas[n].stop_uid,   su->valuestring, sizeof(etas[n].stop_uid)-1);
            strncpy(etas[n].route_uid,  ru->valuestring, sizeof(etas[n].route_uid)-1);
            strncpy(etas[n].route_name, name,           sizeof(etas[n].route_name)-1);
            etas[n].stop_status = ss->valueint;
            if(etas[n].stop_status == 0 || etas[n].stop_status == 1){
                etas[n].estimate_time = et->valueint;
            }else{
                etas[n].estimate_time = 1000000;
            }
            n++;
        }
    }
    cJSON_Delete(root);
    printf("[DEBUG] Found %d ETA entries\n", n);
    return n;
}
