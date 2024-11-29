#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "errors.h"
#include "operation_handler.h"
#include "menu_handler.h"

int does_file_exist(FILE *file, char[1] mode)
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
    FILE *input;
    FILE *output;
    int charecter;
    int error;

    input = fopen(pathSource, "r");
    error = does_file_exist(input, "r");
    if (error != ERR_NONE)
        return error;

    output = fopen(pathDestination, "w");
    error = does_file_exist(output, "w");
    if (error != ERR_NONE)
        return error;

    while ((charecter = fgetc(input)) != EOF)
    {
        fputc(charecter, output);
    }
    fclose(input);

    werase(windowMain);
    wprintw(windowMain, "File Copied Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_create(char pathSource[150])
{
    FILE *file;
    int error;

    file = fopen(pathSource, "w");
    error = does_file_exist(file, "w");
    if (error != ERR_NONE)
        return error;
    fclose(file);

    werase(windowMain);
    wprintw(windowMain, "File Created Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_show(char pathSource[150])
{

    int lineCount = 0, error, ch, lastOperation, index;
    // the width of the window
    char line[WIDTH_OPERATION + 1];
    FILE *file;

    file = fopen(pathSource, "r");
    error = does_file_exist(file, "r");

    if (error != ERR_NONE)
        return error;

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
            return ERR_NONE;
        }
    }
}
// returns ERR_NONE if the operation is successful
int file_delete(char pathSource[150])
{
    if (remove(pathSource) == 0)
        return ERR_NONE;

    werase(windowMain);
    wprintw(windowMain, "File Deleted Successfully!");
    wrefresh(windowMain);
    wgetch(windowMain);

    return ERR_FILE_NOT_FOUND;
}