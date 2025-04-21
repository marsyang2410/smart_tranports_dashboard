// http.h
#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>   // ← for size_t

int http_get(const char* url,
             const char* bearer_token,
             char* response,
             size_t response_len);

#endif // HTTP_H
