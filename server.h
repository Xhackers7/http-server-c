#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "config.h"
#include "file.h"
#include "parser.h"

#define PROTOCOL "HTTP/1.1 "

#define STATUS_200 PROTOCOL "200 OK"
#define STATUS_201 PROTOCOL "201 Created"
#define STATUS_202 PROTOCOL "202 Accepted"

#define STATUS_400 PROTOCOL "400 Bad Request"
#define STATUS_401 PROTOCOL "401 Unauthorized"
#define STATUS_402 PROTOCOL "402 Payment Required"
#define STATUS_403 PROTOCOL "403 Forbidden"
#define STATUS_404 PROTOCOL "404 Not Found"

#define STATUS_500 PROTOCOL "500 Internal Server Error"
#define STATUS_501 PROTOCOL "501 Not Implemented"
#define STATUS_502 PROTOCOL "502 Bad Gateway"
#define STATUS_503 PROTOCOL "503 Service Unavailable"
#define STATUS_504 PROTOCOL "504 Gateway Timeout"
#define STATUS_505 PROTOCOL "505 HTTP Version Not Supported"

#define HTML_FILE "text/html"
#define CSS_FILE "text/css"
#define JS_FILE "text/javascript"
#define JSM_FILE "text/javascript"
#define JSON_FILE "application/json"
#define GIF_FILE "image/gif"
#define ICO_FILE "image/x-icon"
#define JPG_FILE "image/jpeg"
#define MP3_FILE "audio/mpeg"
#define MP4_FILE "video/mp4"
#define MPEG_FILE "video/mpeg"
#define PNG_FILE "image/png"
#define PHP_FILE "application/x-httpd-php"
#define SVG_FILE "image/svg+xml"
#define TEXT_FILE "text/plain"
#define BIN_FILE "application/octet-stream"

#define SERVER "Ivan (Alpha 0.24.5.4)"

#define HTTP_HEADER(status, filetype) parse_HTTP_HEADER(status, SERVER, filetype);

typedef struct HTTP_SERVER
{
    int fd;
    struct sockaddr_in host_addr;

} HTTP_SERVER;

HTTP_SERVER *create_server();

int bind_port(HTTP_SERVER *server, unsigned short port);


/*
    Tries to read the file provided and send it to the client
    - sends hardcodded 404 page if file not found or can't be read
*/
int send_file(int client_fp, const char *status, const char *filepath, const char *filetype);
