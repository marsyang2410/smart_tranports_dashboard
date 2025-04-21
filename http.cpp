// http.cpp
#include "http.h"
#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static size_t write_cb(void* ptr, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    strncat((char*)userp, (char*)ptr, total);
    return total;
}

int http_get(const char* url,
             const char* bearer_token,
             char* response,
             size_t response_len)
{
    printf("[DEBUG] HTTP GET %s\n", url);
    printf("[DEBUG] Authorization: Bearer %.20s…\n", bearer_token);

    CURL* curl = curl_easy_init();
    if (!curl) return -1;

    // ─── ENABLE FULL CURL VERBOSE LOGGING ─────────────────────────
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    // (optional) you can also set a custom debug callback:
    // curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_debug_cb);
    // curl_easy_setopt(curl, CURLOPT_DEBUGDATA,    your_debug_data);

    struct curl_slist* headers = NULL;
    // make sure the buffer can hold your whole token + prefix
    size_t needed = strlen(bearer_token) + strlen("Authorization: Bearer ") + 1;
    char *auth_header = (char*)malloc(needed);
    if (!auth_header) return -1;
    snprintf(auth_header, needed,
             "Authorization: Bearer %s", bearer_token);
    headers = curl_slist_append(headers, auth_header);
    free(auth_header);
    headers = curl_slist_append(headers, "Accept: application/json");
    

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "BusTimer/1.0");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    CURLcode res = curl_easy_perform(curl);
    long code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    // Print the response body anytime it’s not a 2xx
    if (code < 200 || code >= 300) {
        printf("[DEBUG] Response body: %s\n", response);
    }
    printf("[DEBUG] HTTP status: %ld\n", code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return (res == CURLE_OK && code >= 200 && code < 300) ? 0 : -1;
}
