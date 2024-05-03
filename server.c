#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include "log.h"

HTTP_SERVER *create_server()
{
    HTTP_SERVER *server = (HTTP_SERVER *)malloc(sizeof(HTTP_SERVER));
    server->fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET (IPv4 connection), SOCK_STREAM (tcp protoccol)
    if (server->fd < 0)
        return NULL;
    server->host_addr.sin_addr.s_addr = INADDR_ANY; // Allow any ip to connect
    return server;
}

int bind_port(HTTP_SERVER *server, unsigned short port)
{
    server->host_addr.sin_family = AF_INET;
    server->host_addr.sin_port = htons(port);
    return bind(server->fd, (struct sockaddr *)&(server->host_addr), sizeof(server->host_addr));
}

int send_file(int client_fd, const char *status, const char *filepath, const char *filetype)
{
    const FileData *file = read_file(filepath);

    if (!file)
    {
        const char *header = HTTP_HEADER(STATUS_404, "text/html");
        send(client_fd, header, strlen(header), 0);
        const char *page = "<html><h1>404 - Page not found</h1></html>";
        send(client_fd, page, strlen(page), 0);
        return -1;
    }

    const char *header = HTTP_HEADER(status, filetype);
    if (header == NULL)
        return -2;
    send(client_fd, header, strlen(header), 0);
    send(client_fd, file->data, file->size, 0);
    return 0;
}
