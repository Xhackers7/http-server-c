#include "parser.h"

const char *get_file_extension(const char *file_path)
{
    const char *dot = strrchr(file_path, '.');
    if (!dot || dot == file_path)
        return "";
    return dot + 1;
}

void parseURI(char *filepath, char *filetype, const char *routePath)
{
    const char *fileEXT = get_file_extension(routePath);
    if (!strcmp(fileEXT, "html"))
        strcpy(filetype, HTML_FILE);
    else if (!strcmp(fileEXT, "htm"))
        strcpy(filetype, HTML_FILE);
    else if (!strcmp(fileEXT, "css"))
        strcpy(filetype, CSS_FILE);
    else if (!strcmp(fileEXT, "js"))
        strcpy(filetype, JS_FILE);
    else if (!strcmp(fileEXT, "ico"))
        strcpy(filetype, ICO_FILE);
    else if (!strcmp(fileEXT, "png"))
        strcpy(filetype, PNG_FILE);
    else if (!strcmp(fileEXT, "jpg"))
        strcpy(filetype, JPG_FILE);
    else if (!strcmp(fileEXT, "jpeg"))
        strcpy(filetype, JPG_FILE);
    else if (!strcmp(routePath, "/"))
    {
        strcpy(filepath, "public/index.html");
        strcpy(filetype, HTML_FILE);
        return;
    }
    else
    {
        strcpy(filetype, TEXT_FILE);
    }
    strcpy(filepath, "public");
    strcat(filepath, routePath);
}

const char *parse_HTTP_HEADER(const char *status, const char *server, const char *filetype)
{
    size_t header_length = 1024;

    char *header = (char *)malloc(header_length);
    if (header == NULL)
    {
        Log_error("PARSE_RESPONSE", "Memory allocation failed");
        return NULL;
    }
    snprintf(header, header_length, "%s\r\nServer: %s\r\nContent-type: %s\r\n\r\n", status, server, filetype);
    Log_debug("Parser", "Header\n%s", header);
    return header;
}