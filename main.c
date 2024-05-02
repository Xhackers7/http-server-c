#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "log.h"

typedef struct HTTP_SERVER
{
    int fd;
    struct sockaddr_in host_addr;

} HTTP_SERVER;

HTTP_SERVER *create_server()
{
    HTTP_SERVER *server = (HTTP_SERVER *)malloc(sizeof(HTTP_SERVER));
    server->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->fd < 0)
        return NULL;
    server->host_addr.sin_addr.s_addr = INADDR_ANY;
    return server;
}

int bind_server(HTTP_SERVER *server, unsigned short port)
{
    server->host_addr.sin_family = AF_INET;
    server->host_addr.sin_port = htons(port);
    return bind(server->fd, (struct sockaddr *)&(server->host_addr), sizeof(server->host_addr));
}


int main()
{
    setLogLevel(level_debug);

    unsigned short port = 3030;

    HTTP_SERVER *server = create_server();
    if (bind_server(server, port) < 0)
        Log_critical("Server", "Unable to bind to port %d", port);
    Log_debug("Server", "Binded to port %d", port);

    if (listen(server->fd, SOMAXCONN) < 0)
        Log_critical("Server", "Failed to start listening");
    Log_info("Server", "Server started at port %d and is listening for connections", port);

    while (1)
    {
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        int client_fd = accept(server->fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
        if (client_fd < 0)
        {
            Log_warning("Server", "Failed to accept connection");
            continue;
        }
        Log_info("Server", "Connect with client %s:%d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        send(client_fd, "Hello ^^", strlen("Hello ^^"), 0);
        close(client_fd);
    }
}