#include "server.h"
#include <stdio.h>
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

const char *parse_HTTP_RESPONSE(const char *status, const char *server, const char *filetype, const char *data)
{
    size_t response_length = strlen(status) + strlen("\r\nServer: ") + strlen(server) +
                             strlen("\r\nContent-type: ") + strlen(filetype) + strlen("\r\n\r\n") + strlen(data) + 1;

    char *response = (char *)malloc(response_length);
    if (response == NULL)
    {
        Log_error("PARSE_RESPONSE", "Memory allocation failed");
        return NULL;
    }

    snprintf(response, response_length, "%s\r\nServer: %s\r\nContent-type: %s\r\n\r\n%s", status, server, filetype, data);

    return response;
}

const char *read_file(const char *filepath)
{
    FILE *fp;
    char *buffer;
    long file_size;
    size_t result;

    fp = fopen(filepath, "rb");

    if (fp == NULL)
    {
        Log_warning("READ_FILE", "Client requsted for file %s but is not found", filepath);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    buffer = (char *)malloc(file_size + 2);

    if (buffer == NULL)
    {
        Log_error("READ_FILE", "Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    result = fread(buffer, 1, file_size, fp);
    if (result != file_size)
    {
        Log_warning("READ_FILE", "Cannot read file %s", filepath);
        fclose(fp);
        free(buffer);
        return NULL;
    }

    buffer[file_size - 1] = '\r';
    buffer[file_size] = '\n';

    fclose(fp);
    return (const char *)buffer;
}

const char *get_file_extension(const char *file_path)
{
    const char *dot = strrchr(file_path, '.');
    if (!dot || dot == file_path)
        return "";
    return dot + 1;
}

void parseURI(char *dst, char *filetype, const char *src)
{
    const char *fileEXT = get_file_extension(src);
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
    else if (!strcmp(src, "/"))
    {
        strcpy(dst, "index.html");
        strcpy(filetype, HTML_FILE);
        return;
    }
    else
    {
        strcpy(filetype, TEXT_FILE);
    }
    strcpy(dst, src + 1);
}

int send_file(int client_fp, const char *status, const char *filepath, const char *filetype)
{
    const char *data = read_file(filepath);
    if (!data)
    {
        if (status == STATUS_404)
        {
            const char *resp = HTTP_RESPONSE(STATUS_404, "<html><h1>404 - Page not found</h1></html>", "text/html");
            send(client_fp, resp, strlen(resp), 0);
        }
        if (!strcmp(filetype, HTML_FILE))
            send_file(client_fp, STATUS_404, "404.html", "text/html");
        else
            send(client_fp, STATUS_404, strlen(STATUS_404), 0);
        return -1;
    }
    const char *resp = HTTP_RESPONSE(status, filetype, data);
    if (resp == NULL)
        return -2;
    send(client_fp, resp, strlen(resp), 0);
    return 0;
}
