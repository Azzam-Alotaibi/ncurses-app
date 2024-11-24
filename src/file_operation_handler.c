#include <stdio.h>
#include <errno.h>
#include <errors.h>
#include "operation_handler.h"

int fileCopy(char filePath[], char destination[])
{
    FILE *in;
    FILE *out;
    int ch;

    in = fopen(filePath, "r");
    if (in == NULL)
    {
        // [10]
        if (errno == ENOENT)
            return ERR_FILE_NOT_FOUND;
        if (errno == EACCES)
            return ERR_PERMISSION_DENIED;
        else
            return ERR_UNKNOWN;
    }
    out = fopen(destination, "w");
    if (out == NULL)
    {
        if (errno == ENOENT)
            return ERR_FOLDER_NOT_FOUND;
        else
            return ERR_UNKNOWN;
    }

    while ((ch = fgetc(in)) != EOF)
    {
        fputc(ch, out);
    }
    fclose(in);

    return ERR_NONE;
}