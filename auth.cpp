// auth.cpp
#include "auth.h"

static size_t write_cb(void* ptr, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    strncat((char*)userp, (char*)ptr, total);
    return total;
}


int get_oauth_token(char* token, size_t token_len)
{

    const char* client_id     = getenv("TDX_CLIENT_ID");
    const char* client_secret = getenv("TDX_CLIENT_SECRET");
    if (!client_id || !client_secret) {
        fprintf(stderr, "Missing TDX credentials\n");
        return -1;
    }

    printf("[DEBUG] Getting OAuth token...\n");
    CURL* curl = curl_easy_init();
    if (!curl) return -1;

    char postfields[512] = {0};
    snprintf(postfields, sizeof(postfields),
             "grant_type=client_credentials&client_id=%s&client_secret=%s",
             client_id, client_secret);

    printf("[DEBUG] POST https://tdx.transportdata.tw/auth/realms/TDXConnect/protocol/openid-connect/token\n");
    printf("[DEBUG] Body: %s\n", postfields);

    curl_easy_setopt(curl, CURLOPT_URL,
        "https://tdx.transportdata.tw/auth/realms/TDXConnect/protocol/openid-connect/token");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, token);

    CURLcode res = curl_easy_perform(curl);
    printf("[DEBUG] curl status: %d\n", res);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) return -1;

    // parse JSON to extract "access_token"
    cJSON* root = cJSON_Parse(token);
    if (!root) return -1;
    cJSON* at = cJSON_GetObjectItem(root, "access_token");
    if (!at) { cJSON_Delete(root); return -1; }
    strncpy(token, at->valuestring, token_len - 1);
    cJSON_Delete(root);

    printf("[DEBUG] Received token: %20s\n", token);
    return 0;
}
