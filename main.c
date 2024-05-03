#include <stdio.h>
#include "log.h"

#include "server.h"

#define BUFFSIZE 1024

void get_handler(int client_fd, const char *filepath, const char *filetype)
{
    send_file(client_fd, STATUS_200, filepath, filetype);
}
void post_handler(int client_fd, const char *filepath, const char *filetype)
{

}


int main()
{
    setLogLevel(level_debug);

    unsigned short port = PORT;

    HTTP_SERVER *server = create_server();
    if (bind_port(server, port) < 0)
        Log_critical("Server", "Unable to bind to port %d", port);
    Log_debug("Server", "Binded to port %d", port);

    // Turn the server into passive mode (listening for connections)
    if (listen(server->fd, SOMAXCONN) < 0)
        Log_critical("Server", "Failed to start listening");
    Log_info("Server", "Server started at port %d and is listening for connections", port);

    while (1)
    {
        // Wait for a connection
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        int client_fd = accept(server->fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
        if (client_fd < 0)
        {
            Log_warning("Server", "Failed to accept connection");
            continue;
        }

        Log_debug("Server", "Connect with client %s:%d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Creates a child(clone)    of the server to handle multiple clients
        if (fork() == 0)
        {
            Log_debug("Server", "Child created");

            char buffer[BUFFSIZE];
            close(server->fd);

            if (recv(client_fd, buffer, BUFFSIZE, 0) < 0)
            {
                Log_warning("Server", "Invalid request");
                break;
            }

            char method[BUFFSIZE], uri[BUFFSIZE], version[BUFFSIZE];
            char *filepath = malloc(BUFFSIZE);
            char *filetype = malloc(BUFFSIZE);
            sscanf(buffer, "%s %s %s", method, uri, version);
            parseURI(filepath, filetype, uri);
            Log_debug("Server","[%s:%u] %s %s %s\n", inet_ntoa(client_addr.sin_addr),
                   ntohs(client_addr.sin_port), method, version, uri);
            
            Log_debug("Server", "%s %s", filepath, filetype);

            if (!strcmp(method, "GET"))  get_handler(client_fd, filepath, filetype);
            if (!strcmp(method, "POST")) post_handler(client_fd, filepath, filetype);

            // send_file(client_fd, STATUS_200, "test1.html", HTML_FILE);
            close(client_fd);
            break;
        }
        close(client_fd);
    }
    free(server);
    Log_debug("Server", "Child destroyed");
    return 0;
}