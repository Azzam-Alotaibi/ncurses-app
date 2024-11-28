#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "errors.h"
#include "operation_handler.h"
#include "menu_handler.h"

int does_file_exist(FILE *file)
{
    if (file == NULL)
    {
        // [10]
        if (errno == ENOENT)
            return ERR_FILE_NOT_FOUND;
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
    error = does_file_exist(input);
    if (error != ERR_NONE)
        return error;

    output = fopen(pathDestination, "w");
    error = does_file_exist(output);
    if (error != ERR_NONE)
        return error;

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

    FILE *file = fopen(pathSource, "w");
    int error = does_file_exist(file);
    if (error != ERR_NONE)
        return error;
    fclose(file);

    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_show(char pathSource[150])
{
    int lineCount = 0, error, cursorYPosition = 0, ch, lastOperation, index;
    // the width of the window
    char line[WIDTH_OPERATION + 1];
    FILE *file;

    file = fopen(pathSource, "r");
    error = does_file_exist(file);

    if (error != ERR_NONE)
        return error;

    // destroy_window();

    while (fgets(line, sizeof(line), file))
    {
        lineCount++;
    }

    // to reset the pointer to the start of the file
    fseek(file, 0, SEEK_SET);

    werase(windowMain);
    wrefresh(windowMain);
    if (lineCount <= HEIGHT_OPERATION)
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
        strcpy(fileContent[i], line);
    }
    fclose(file);

    while (cursorYPosition < HEIGHT_OPERATION - 1)
    {
        mvwprintw(windowMain, cursorYPosition, 0, "%s", fileContent[cursorYPosition]);
        cursorYPosition++;
    }
    lastOperation = KEY_DOWN;
    index = cursorYPosition;
    while (true)
    {
        ch = wgetch(windowMain);
        //! super buggy
        switch (ch)
        {
        case KEY_UP:
            if (lastOperation == KEY_DOWN)
                index -= HEIGHT_OPERATION - 1;
            lastOperation = KEY_UP;
            if (index == 0)
                break;
            cursorYPosition--;
            index--;
            // scroll up
            wscrl(windowMain, -1);
            mvwprintw(windowMain, cursorYPosition, 0, "%s", fileContent[index]);

            break;
        case KEY_DOWN:
            if (lastOperation == KEY_UP)
                index += HEIGHT_OPERATION;
            lastOperation = KEY_DOWN;
            if (index == lineCount - 1)
                break;
            cursorYPosition++;
            index++;
            // scroll down
            wscrl(windowMain, 1);
            mvwprintw(windowMain, cursorYPosition, 0, "%s", fileContent[index]);

            break;
        case 'q':
            return ERR_NONE;
        }
        wrefresh(windowMain);
    }
}
// returns ERR_NONE if the operation is successful
int file_delete(char pathSource[150])
{
    // todo delete file
    return ERR_NONE;
}