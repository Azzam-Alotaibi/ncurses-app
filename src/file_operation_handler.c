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

int file_copy(char pathSource[150], char pathDestination[150])
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
int file_create(char pathSource[150])
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
int file_show(char pathSource[150])
{

    int lineCount = 0, error, ch, lastOperation, index;
    // the width of the window
    char line[WIDTH_OPERATION + 1], logMessage[300];
    FILE *file;

    file = fopen(pathSource, "r");
    error = does_file_exist(file, "r");

    if (error != ERR_NONE)
    {
        fclose(file);
        return error;
    }

    // destroy_window();

    while (fgets(line, sizeof(line), file))
    {
        lineCount++;
    }

    // to reset the pointer to the start of the file
    fseek(file, 0, SEEK_SET);

    // deletes
    werase(windowMain);
    wrefresh(windowMain);
    if (lineCount < HEIGHT)
    {
        while (fgets(line, sizeof(line), file))
        {
            wprintw(windowMain, "%s", line);
        }
        wgetch(windowMain);
        fclose(file);
        lineCount += 1;
        snprintf(logMessage, sizeof(logMessage), "you showed the file: \"%s\", and it had %d lines", pathSource, lineCount);
        log_operation(logMessage);
        return ERR_NONE;
    }

    char fileContent[lineCount][WIDTH_OPERATION + 1];
    for (int i = 0; i < lineCount; i++)
    {
        fgets(line, sizeof(line), file);

        // this line replaces the the first "\n" occurence with null "\0".
        // the way fgets work, is to stop if it encounters a new line "\n" or the sizeof(line) is all used up.
        // hence, the new line "\n" was sabotaging the custom scroll implementation so I had to delete it
        // [13]
        strtok(line, "\n");
        strcpy(fileContent[i], line);
    }
    fclose(file);
    for (int i = 0; i < HEIGHT; i++)
    {
        mvwprintw(windowMain, i, 0, "%s", fileContent[i]);
    }
    // mvwprintw(windowMain, HEIGHT - 1, 0, "%s", fileContent[HEIGHT - 1]);

    lastOperation = KEY_DOWN;
    index = HEIGHT - 1;
    // custom scrolling in the terminal
    while (true)
    {
        ch = wgetch(windowMain);

        switch (ch)
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

            break;
        case 'q':
            lineCount += 1;
            snprintf(logMessage, sizeof(logMessage), "you showed the file: \"%s\", and it had %d lines", pathSource, lineCount);
            log_operation(logMessage);
            return ERR_NONE;
        }
    }
}
// returns ERR_NONE if the operation is successful
int file_delete(char pathSource[150])
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

    snprintf(logMessage, sizeof(logMessage), "you deleted the file: \"%s\", and it had %d lines", pathSource, lineCount);
    log_operation(logMessage);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_count_lines(char pathSource[150])
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

    snprintf(logMessage, sizeof(logMessage), "you counted the lines of the file: \"%s\", and it had %d lines", pathSource, lineCount);
    log_operation(logMessage);
    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_count_lines_without_window(char pathSource[150])
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
