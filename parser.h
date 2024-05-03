#pragma once
#include <string.h>
#include "server.h"
#include "log.h"

const char *get_file_extension(const char *file_path);

/*
    Extracts the filetype and sets filepath to public/<route-path>
    - routePath begins with / which is ignored
    - if routePath is just / filepath is set to public/index.html and filetype is set to text/html
    - unknown filetypes are set to text/plain by default
*/
void parseURI(char *filepath, char *filetype, const char *routePath);

const char *parse_HTTP_HEADER(const char *status, const char *server, const char *filetype);