#include <stdio.h>
#include <errno.h>
#include "errors.h"
#include "operation_handler.h"

int file_copy(char pathSource[150], char pathDestination[150])
{
    FILE *input;
    FILE *output;
    int charecter;
    input = fopen(pathSource, "r");
    if (input == NULL)
    {
        // [10]
        if (errno == ENOENT)
            return ERR_FILE_NOT_FOUND;
        if (errno == EACCES)
            return ERR_PERMISSION_DENIED;
        else
            return ERR_UNKNOWN;
    }
    output = fopen(pathDestination, "w");
    if (output == NULL)
    {
        if (errno == ENOENT)
            return ERR_FOLDER_NOT_FOUND;
        else
            return ERR_UNKNOWN;
    }

    while ((charecter = fgetc(input)) != EOF)
    {
        fputc(charecter, output);
    }
    fclose(input);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_create(char pathSource[150])
{
    // todo create file
    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_show(char pathSource[150])
{
    // todo show file
    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_delete(char pathSource[150])
{
    // todo delete file
    return ERR_NONE;
}