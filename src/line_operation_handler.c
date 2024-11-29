#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "errors.h"
#include "menu_handler.h"
#include "operation_handler.h"

// returns ERR_NONE if the operation is successful
int line_append(char pathSource[150], const char *newLine)
{
    FILE *file;
    char line[128], tempFileName[] = "_temp.txt";
    int error, currentLine = 1;

    file = fopen(pathSource, "a");
    error = does_file_exist(file, "a");
    if (error != ERR_NONE)
        return error;

    // appending the new line to the file
    fprintf(file, "%s\n", newLine);
    fclose(file);
    // todo append line
    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int line_insert(char pathSource[150], int lineNumber, const char *newLine)
{

    FILE *fileMain, *fileTemp;
    char line[128], tempFileName[] = "_temp.txt";
    int error, currentLine = 1;

    // [15]
    fileMain = fopen(pathSource, "r");
    error = does_file_exist(fileMain, "r");
    if (error != ERR_NONE)
    {
        return error;
    }
    fileTemp = fopen(tempFileName, "w");
    while (fgets(line, sizeof(line), fileMain))
    {

        if (currentLine == lineNumber)
        {
            // since the last line don't have \n, to stay consistent, we have to remove it and always add it.
            strtok(line, "\n");
            fprintf(fileTemp, "%s\n%s\n", line, newLine);
        }
        else
        {
            fprintf(fileTemp, "%s", line);
        }
        currentLine++;
    }

    fclose(fileMain);
    fclose(fileTemp);

    // replace the main file with the temporary file that have been modified and
    remove(pathSource);
    rename(tempFileName, pathSource);

    if (currentLine < lineNumber)
        return ERR_OUT_OF_BOUNDS;

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int line_delete(char pathSource[80], int lineNumber)
{
    FILE *fileMain, *fileTemp;
    char line[128], tempFileName[] = "_temp.txt";
    int error, currentLine = 1;

    // [15]
    fileMain = fopen(pathSource, "r");
    error = does_file_exist(fileMain, "r");
    if (error != ERR_NONE)
    {
        return error;
    }

    fileTemp = fopen(tempFileName, "w");
    while (fgets(line, sizeof(line), fileMain))
    {

        if (currentLine != lineNumber)
        {
            fprintf(fileTemp, "%s", line);
        }
        currentLine++;
    }

    fclose(fileMain);
    fclose(fileTemp);

    // replace the main file with the temporary file that have been modified and
    remove(pathSource);
    rename(tempFileName, pathSource);

    if (currentLine < lineNumber)
        return ERR_OUT_OF_BOUNDS;

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int line_show(char pathSource[80], int lineNumber)
{
    FILE *fileMain;
    char line[128];
    int error, currentLine = 1;

    // [15]
    fileMain = fopen(pathSource, "r");
    error = does_file_exist(fileMain, "r");
    if (error != ERR_NONE)
    {
        return error;
    }

    while (fgets(line, sizeof(line), fileMain))
    {

        if (currentLine == lineNumber)
        {
            werase(windowMain);
            wprintw(windowMain, "%s", line);
            wrefresh(windowMain);
            wgetch(windowMain);
            fclose(fileMain);
            return ERR_NONE;
        }
        currentLine++;
    }

    fclose(fileMain);

    return ERR_OUT_OF_BOUNDS;
}
