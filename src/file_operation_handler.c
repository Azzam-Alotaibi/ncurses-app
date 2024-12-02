#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "errors.h"
#include "operation_handler.h"
#include "window_handler.h"

int does_file_exist(FILE *file, char mode[1])
{

    // [14]
    if (file == NULL)
    {
        // [10]
        if (errno == ENOENT)
        {
            if (strcmp(mode, "r") == 0)
                return ERR_FILE_NOT_FOUND;
            return ERR_FOLDER_NOT_FOUND;
        }
        if (errno == EACCES)
            return ERR_PERMISSION_DENIED;
        else
            return ERR_UNKNOWN;
    }
    return ERR_NONE;
}

int file_copy(char *pathSource, char pathDestination[150])
{
    FILE *input, *output;
    int charecter, error, lineCount;
    char logMessage[400];

    input = fopen(pathSource, "r");
    error = does_file_exist(input, "r");
    if (error != ERR_NONE)
    {
        fclose(input);
        return error;
    }

    output = fopen(pathDestination, "w");
    error = does_file_exist(output, "w");
    if (error != ERR_NONE)
    {
        fclose(input);
        fclose(output);
        return error;
    }

    while ((charecter = fgetc(input)) != EOF)
    {
        fputc(charecter, output);
    }
    fclose(input);
    fclose(output);

    werase(windowMain);
    wprintw(windowMain, "File Copied Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    lineCount = file_count_lines_without_window(pathSource);
    snprintf(logMessage, sizeof(logMessage), "copied the file: \"%s\" to the destination: \"%s\", and it had %d lines", pathSource, pathDestination, lineCount);
    log_operation(logMessage);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_create(char *pathSource)
{
    FILE *file;
    int error, lineCount;
    char logMessage[300];

    file = fopen(pathSource, "w");
    error = does_file_exist(file, "w");
    if (error != ERR_NONE)
    {
        fclose(file);
        return error;
    }
    fclose(file);

    werase(windowMain);
    wprintw(windowMain, "File Created Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    lineCount = file_count_lines_without_window(pathSource);
    snprintf(logMessage, sizeof(logMessage), "created the file: \"%s\", and it had %d lines", pathSource, lineCount);
    log_operation(logMessage);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_show(char *pathSource)
{

    int lineCount = 0, error, character, lastOperation, index, lineWidth = 0;
    // the width of the window
    // buffers holds a chunck of data one at a time to reduce I/O system operations (input and output like printing and reading from a file)
    char buffer[WIDTH_OPERATION], logMessage[300];
    FILE *file;

    file = fopen(pathSource, "r");
    error = does_file_exist(file, "r");

    if (error != ERR_NONE)
    {
        fclose(file);
        return error;
    }

    // destroy_window();

    while ((character = fgetc(file)) != EOF)
    {
        lineWidth++;
        if (lineWidth == WIDTH_OPERATION - 1)
        {
            lineWidth = 0;
            lineCount++;
        }
        else if (character == '\n')
        {
            lineWidth = 0;
            lineCount++;
        }
    }

    // to reset the pointer to the start of the file
    fseek(file, 0, SEEK_SET);

    // deletes the previuos screen content
    werase(windowMain);
    wrefresh(windowMain);
    // if the screen doesn't need to scroll
    if (lineCount <= HEIGHT)
    {
        while (fgets(buffer, sizeof(buffer), file))
        {
            wprintw(windowMain, "%s", buffer);
        }
        wgetch(windowMain);
        fclose(file);
        lineCount += 1;
        snprintf(logMessage, sizeof(logMessage), "showed the file: \"%s\", and it had %d lines", pathSource, lineCount);
        log_operation(logMessage);
        return ERR_NONE;
    }

    // in this section, the screen needs to scroll

    char fileContent[lineCount][WIDTH_OPERATION];
    for (int i = 0; i < lineCount; i++)
    {
        fgets(buffer, sizeof(buffer), file);

        // this line replaces the the first "\n" occurence with null "\0".
        // the way fgets work, is to stop if it encounters a new line "\n" or the sizeof(line) is all used up.
        // hence, the new line "\n" was sabotaging the custom scroll implementation so I had to delete it
        // [13]
        strtok(buffer, "\n");
        strcpy(fileContent[i], buffer);
    }
    fclose(file);
    for (int i = 0; i < HEIGHT; i++)
    {
        mvwprintw(windowMain, i, 0, "%s", fileContent[i]);
        wrefresh(windowMain);
    }
    // mvwprintw(windowMain, HEIGHT - 1, 0, "%s", fileContent[HEIGHT - 1]);

    lastOperation = KEY_DOWN;
    index = HEIGHT - 1;
    // custom scrolling in the terminal
    while (true)
    {
        character = wgetch(windowMain);

        switch (character)
        {
        case KEY_UP:
            if (lastOperation == KEY_DOWN)
                index -= HEIGHT - 1;

            lastOperation = KEY_UP;
            if (index == 0)
                break;
            index--;
            // scroll up
            wscrl(windowMain, -1);
            mvwprintw(windowMain, 0, 0, "%s", fileContent[index]);

            break;
        case KEY_DOWN:
            if (lastOperation == KEY_UP)
                index += HEIGHT - 1;
            lastOperation = KEY_DOWN;
            if (index == lineCount - 1)
                break;
            index++;
            // scroll down
            wscrl(windowMain, 1);
            mvwprintw(windowMain, HEIGHT - 1, 0, "%s", fileContent[index]);
            wrefresh(windowMain);

            break;
        case 'q':
            lineCount += 1;
            snprintf(logMessage, sizeof(logMessage), "showed the file: \"%s\", and it had %d lines", pathSource, lineCount);
            log_operation(logMessage);
            return ERR_NONE;
        }
    }
}
// returns ERR_NONE if the operation is successful
int file_delete(char *pathSource)
{
    char logMessage[300];
    int lineCount;

    lineCount = file_count_lines_without_window(pathSource);

    if (remove(pathSource) != 0)
        return ERR_FILE_NOT_FOUND;

    werase(windowMain);
    wprintw(windowMain, "File Deleted Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    snprintf(logMessage, sizeof(logMessage), "deleted the file: \"%s\", and it had %d lines", pathSource, lineCount);
    log_operation(logMessage);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_count_lines(char *pathSource)
{
    FILE *file = fopen(pathSource, "r");
    char character, logMessage[300];
    int lineCount = 1, error;

    error = does_file_exist(file, "r");
    if (error != ERR_NONE)
    {
        fclose(file);

        return error;
    }

    while ((character = fgetc(file)) != EOF)
    {
        if (character == '\n')
            lineCount++;
    }
    fclose(file);

    werase(windowMain);
    wprintw(windowMain, "The number of lines in the file: %d", lineCount);
    wrefresh(windowMain);
    wgetch(windowMain);

    snprintf(logMessage, sizeof(logMessage), "counted the lines of the file: \"%s\", and it had %d lines", pathSource, lineCount);
    log_operation(logMessage);
    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_count_lines_without_window(char *pathSource)
{
    FILE *file = fopen(pathSource, "r");
    char character, logMessage[300];
    int lineCount = 1, error;

    error = does_file_exist(file, "r");
    if (error != ERR_NONE)
    {
        fclose(file);
        return error;
    }

    while ((character = fgetc(file)) != EOF)
    {
        if (character == '\n')
            lineCount++;
    }
    fclose(file);

    return lineCount;
}

int file_encrypt(char *pathSource, char *key)
{
    FILE *fileMain, *fileTemp;
    int charecter, error, lineCount, keyIndex = 0, keyLength;
    char logMessage[400], fileTempName[] = "_temp.txt";

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

    keyLength = strlen(key);

    while ((charecter = fgetc(fileMain)) != EOF)
    {
        // [16]
        fputc(charecter ^ key[keyIndex], fileTemp);

        keyIndex = (keyIndex + 1) % keyLength;
    }
    fclose(fileMain);
    fclose(fileTemp);

    // replace the main file with the temporary file that have been modified and
    remove(pathSource);
    rename(fileTempName, pathSource);

    werase(windowMain);
    wprintw(windowMain, "File Encrypted Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    lineCount = file_count_lines_without_window(pathSource);
    snprintf(logMessage, sizeof(logMessage), "encrypted the file: \"%s\", and it had %d lines", pathSource, lineCount);
    log_operation(logMessage);

    return ERR_NONE;
}
