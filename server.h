#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "config.h"

#define HTML_FILE "text/html"
#define BIN_FILE  "application/octet-stream"



#define HTTP_RESPONSE(status, filetype, data) parse_HTTP_RESPONSE(status, SERVER, filetype, data);

typedef struct HTTP_SERVER
{
    int fd;
    struct sockaddr_in host_addr;

} HTTP_SERVER;


HTTP_SERVER *create_server();

int bind_port(HTTP_SERVER *server, unsigned short port);


const char *parse_HTTP_RESPONSE(const char *status, const char *server, const char *filetype, const char *data);

const char *read_file(const char *filepath);

int send_file(int client_fp, const char *filepath, const char *filetype);
