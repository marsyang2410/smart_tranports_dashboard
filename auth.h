// auth.h
#ifndef AUTH_H
#define AUTH_H

#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include <stdlib.h>
#include <stddef.h>     // ← for size_t

int get_oauth_token(char* token, size_t token_len);

#endif // AUTH_H
