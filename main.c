#include <stdio.h>
#include "log.h"

#include "server.h"

#define PORT 3032
#define BUFFSIZE 5096


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

        // Creates a clone of the server to handle multiple clients
        if (fork() == 0)
        {
            close(server->fd);
            send_file(client_fd, "test.html", HTML_FILE);
            close(client_fd);
            break;
        }
        close(client_fd);
    }
    free(server);
    return 0;
}