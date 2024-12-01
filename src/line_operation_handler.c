#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "errors.h"
#include "window_handler.h"
#include "operation_handler.h"

// returns ERR_NONE if the operation is successful
int line_append(char *pathSource, char *newLine)
{
    FILE *file;
    char logMessage[300];
    int error, lineCount;

    file = fopen(pathSource, "a");
    error = does_file_exist(file, "a");
    if (error != ERR_NONE)
    {
        fclose(file);
        return error;
    }

    // appending the new line to the file
    fprintf(file, "%s\n", newLine);
    fclose(file);

    werase(windowMain);
    wprintw(windowMain, "Line Appended Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    lineCount = file_count_lines_without_window(pathSource);
    snprintf(logMessage, sizeof(logMessage), "appended a line to the file: \"%s\", and it had %d lines", pathSource, lineCount);
    log_operation(logMessage);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int line_insert(char *pathSource, int lineNumber, char *newLine)
{

    FILE *fileMain, *fileTemp;
    char fileTempName[] = "_temp.txt", logMessage[300];
    int error, currentLine = 1, character;

    // [15]
    fileMain = fopen(pathSource, "r");
    error = does_file_exist(fileMain, "r");
    if (error != ERR_NONE)
    {
        fclose(fileMain);
        return error;
    }
    fileTemp = fopen(fileTempName, "w");
    error = does_file_exist(fileTemp, "w");
    if (error != ERR_NONE)
    {
        fclose(fileMain);
        fclose(fileTemp);
        return error;
    }
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
    rename(fileTempName, pathSource);

    if (currentLine <= lineNumber)
    {
        snprintf(logMessage, sizeof(logMessage), "FAIL: inserted a line to the file: \"%s\", and it had %d lines", pathSource, currentLine);
        log_operation(logMessage);
        return ERR_OUT_OF_BOUNDS;
    }

    werase(windowMain);
    wprintw(windowMain, "Line Inserted Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    snprintf(logMessage, sizeof(logMessage), "inserted a line to the file: \"%s\", and it had %d lines", pathSource, currentLine);
    log_operation(logMessage);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int line_delete(char *pathSource, int lineNumber)
{
    FILE *fileMain, *fileTemp;
    char fileTempName[] = "_temp.txt", logMessage[300];
    int error, currentLine = 1, character;

    // [15]
    fileMain = fopen(pathSource, "r");
    error = does_file_exist(fileMain, "r");
    if (error != ERR_NONE)
    {
        fclose(fileMain);
        return error;
    }

    fileTemp = fopen(fileTempName, "w");
    error = does_file_exist(fileTemp, "w");
    if (error != ERR_NONE)
    {
        fclose(fileMain);
        fclose(fileTemp);

        return error;
    }
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
    rename(fileTempName, pathSource);

    if (currentLine <= lineNumber)
    {
        snprintf(logMessage, sizeof(logMessage), "FAIL: deleted the %d line in the file: \"%s\", and it had %d lines", lineNumber, pathSource, currentLine - 1);
        log_operation(logMessage);
        return ERR_OUT_OF_BOUNDS;
    }

    werase(windowMain);
    wprintw(windowMain, "Line Deleted Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    snprintf(logMessage, sizeof(logMessage), "deleted the %d line in the file: \"%s\", and it had %d lines", lineNumber, pathSource, currentLine - 1);
    log_operation(logMessage);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int line_show(char *pathSource, int lineNumber)
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

    // delete the prev content
    werase(windowMain);

    while ((character = fgetc(fileMain)) != EOF)
    {
        if (currentLine == lineNumber)
        {
            wprintw(windowMain, "%c", character);
            wrefresh(windowMain);
        }
        if (character == '\n')
            currentLine++;
    }
    fclose(fileMain);

    if (lineNumber > currentLine)
    {
        snprintf(logMessage, sizeof(logMessage), "FAIL: you showed the %d line in the file: \"%s\", and it had %d lines", lineNumber, pathSource, currentLine);
        log_operation(logMessage);
        return ERR_OUT_OF_BOUNDS;
    }

    wgetch(windowMain);

    snprintf(logMessage, sizeof(logMessage), "showed the %d line in the file: \"%s\", and it had %d lines", lineNumber, pathSource, currentLine);
    log_operation(logMessage);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int text_replace(char *pathSource, char *textOriginal, char *textNew)
{

    FILE *fileMain, *fileTemp;
    char logMessage[1024], fileTempName[] = "_temp.txt";
    int error, character, index = 0, indexReplace = 0, characterCount = 0, textOriginalSize;

    fileMain = fopen(pathSource, "r");
    error = does_file_exist(fileMain, "r");
    if (error != ERR_NONE)
    {
        fclose(fileMain);
        return error;
    }

    fileTemp = fopen(fileTempName, "w");
    error = does_file_exist(fileTemp, "w");
    if (error != ERR_NONE)
    {
        fclose(fileMain);
        fclose(fileTemp);
        return error;
    }

    textOriginalSize = strlen(textOriginal);

    while ((character = fgetc(fileMain)) != EOF)
    {
        if (character == textOriginal[index])
        {
            if (index == 0)
                indexReplace = characterCount;
            if (index == textOriginalSize - 1)
                break;
            index++;
        }
        else
        {
            index = 0;
        }
        characterCount++;
    }
    if (index != textOriginalSize - 1)
    {

        snprintf(logMessage, sizeof(logMessage), "FAIL: replaced the text: \"%s\" with the text: \"%s\" in the file: \"%s\".", textOriginal, textNew, pathSource);
        log_operation(logMessage);

        fclose(fileMain);
        fclose(fileTemp);

        remove(fileTempName);
        return ERR_TEXT_NOT_FOUND;
    }
    // reset the variables
    characterCount = 0;
    index = 0;
    // to reset the pointer to the start of the file
    fseek(fileMain, 0, SEEK_SET);
    while ((character = fgetc(fileMain)) != EOF)
    {
        if (characterCount == indexReplace)
        {
            if (index == 0)
                fputs(textNew, fileTemp);
            if (index == textOriginalSize - 1)
            {
                characterCount++;
                continue;
            }
            index++;
            indexReplace++;
        }
        else
        {
            fputc(character, fileTemp);
        }
        characterCount++;
    }

    fclose(fileMain);
    fclose(fileTemp);

    // replace the main file with the temporary file that have been modified and
    remove(pathSource);
    rename(fileTempName, pathSource);

    werase(windowMain);
    wprintw(windowMain, "Line Replaced Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    return ERR_NONE;
}