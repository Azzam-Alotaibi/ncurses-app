#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "errors.h"
#include "window_handler.h"
#include "operation_handler.h"

// returns ERR_NONE if the operation is successful
int line_append(char pathSource[150], char *newLine)
{
    FILE *file;
    char line[128], tempFileName[] = "_temp.txt", logMessage[300];
    int error, lineCount;

    file = fopen(pathSource, "a");
    error = does_file_exist(file, "a");
    if (error != ERR_NONE)
        return error;

    // appending the new line to the file
    fprintf(file, "%s\n", newLine);
    fclose(file);

    werase(windowMain);
    wprintw(windowMain, "Line Appended Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    lineCount = file_count_lines_without_window(pathSource);
    snprintf(logMessage, sizeof(logMessage), "you appended a line to the file: \"%s\", and it had %d lines", pathSource, lineCount);
    log_operation(logMessage);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int line_insert(char pathSource[150], int lineNumber, char *newLine)
{

    FILE *fileMain, *fileTemp;
    char tempFileName[] = "_temp.txt", logMessage[300];
    int error, currentLine = 1, character;

    // [15]
    fileMain = fopen(pathSource, "r");
    error = does_file_exist(fileMain, "r");
    if (error != ERR_NONE)
    {
        fclose(fileMain);
        return error;
    }
    fileTemp = fopen(tempFileName, "w");
    while ((character = fgetc(fileMain)) != EOF)
    {
        if (character == '\n')
            currentLine++;
        fputc(character, fileTemp);
        if (currentLine == lineNumber)
        {

            fprintf(fileTemp, "%s\n", newLine);
            currentLine++;
        }
    }

    fclose(fileMain);
    fclose(fileTemp);

    // replace the main file with the temporary file that have been modified and
    remove(pathSource);
    rename(tempFileName, pathSource);

    if (currentLine <= lineNumber)
        return ERR_OUT_OF_BOUNDS;

    werase(windowMain);
    wprintw(windowMain, "Line Inserted Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    snprintf(logMessage, sizeof(logMessage), "you inserted a line to the file: \"%s\", and it had %d lines", pathSource, currentLine);
    log_operation(logMessage);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int line_delete(char pathSource[150], int lineNumber)
{
    FILE *fileMain, *fileTemp;
    char tempFileName[] = "_temp.txt", logMessage[300];
    int error, currentLine = 1, lineLength, character;

    // [15]
    fileMain = fopen(pathSource, "r");
    error = does_file_exist(fileMain, "r");
    if (error != ERR_NONE)
    {
        fclose(fileMain);
        return error;
    }

    fileTemp = fopen(tempFileName, "w");
    while ((character = fgetc(fileMain)) != EOF)
    {
        if (character == '\n')
            currentLine++;

        if (currentLine != lineNumber)
            fputc(character, fileTemp);
    }

    fclose(fileMain);
    fclose(fileTemp);

    // replace the main file with the temporary file that have been modified and
    remove(pathSource);
    rename(tempFileName, pathSource);

    if (currentLine <= lineNumber)
        return ERR_OUT_OF_BOUNDS;

    werase(windowMain);
    wprintw(windowMain, "Line Deleted Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    snprintf(logMessage, sizeof(logMessage), "you deleted the %d line in the file: \"%s\", and it had %d lines", lineNumber, pathSource, currentLine - 1);
    log_operation(logMessage);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int line_show(char pathSource[150], int lineNumber)
{
    FILE *fileMain;
    char logMessage[300];
    int error, currentLine = 1, character;

    // [15]
    fileMain = fopen(pathSource, "r");
    error = does_file_exist(fileMain, "r");
    if (error != ERR_NONE)
    {
        fclose(fileMain);
        return error;
    }

    werase(windowMain);
    while ((character = fgetc(fileMain)) != EOF)
    {
        if (character == '\n')
            currentLine++;

        if (currentLine == lineNumber)
        {
            wprintw(windowMain, "%c", character);
            wrefresh(windowMain);
        }
    }
    fclose(fileMain);

    if (lineNumber > currentLine)
        return ERR_OUT_OF_BOUNDS;

    wgetch(windowMain);

    snprintf(logMessage, sizeof(logMessage), "you showed the %d line in the file: \"%s\", and it had %d lines", lineNumber, pathSource, currentLine);
    log_operation(logMessage);

    return ERR_NONE;
}
