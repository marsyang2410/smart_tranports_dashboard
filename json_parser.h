#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "cJSON.h"

/* Helpers around cJSON */
#define json_parse(s)        cJSON_Parse(s)
#define json_free(root)      cJSON_Delete(root)

#endif // JSON_PARSER_H
