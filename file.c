#include "file.h"

const FileData *read_file(const char *filepath)
{
    FILE *fp;
    FileData *file = malloc(sizeof(FileData));
    long file_size;

    fp = fopen(filepath, "rb");

    if (fp == NULL)
    {
        Log_warning("READ_FILE", "Client requsted for file %s but is not found", filepath);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    file->data = (char *)malloc(file_size + 1);

    if (file->data == NULL)
    {
        Log_error("READ_FILE", "Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    file->size = fread(file->data, 1, file_size, fp);
    if (file->size != file_size)
    {
        Log_warning("READ_FILE", "Cannot read file %s", filepath);
        fclose(fp);
        free(file->data);
        return NULL;
    }

    file->data[file_size] = '\0';

    fclose(fp);
    return (const FileData *)file;
}