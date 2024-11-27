#include <stdio.h>
#include <errno.h>
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
    int lineCount, error, cursorYPosition = 0, ch;
    char line[WIDTH_OPERATION - 1];
    FILE *file;

    file = fopen(pathSource, "r");
    error = does_file_exist(file);

    if (error != ERR_NONE)
        return error;

    // destroy_window();
    lineCount = 0;
    while (fgets(line, sizeof(line), file))
    {
        lineCount++;
    }
    // to reset the pointer to the start of the file
    fseek(file, 0, SEEK_SET);

    werase(windowMain);
    wrefresh(windowMain);
    if (lineCount > HEIGHT_MENU)
    {

        // scrollok(windowMain, true);
        // scrollok(windowBorder, true);
        destroy_window(windowBorder);

        setup_window_operation(lineCount);
    }
    while (fgets(line, sizeof(line), file))
    {
        mvwprintw(windowMain, cursorYPosition++, 0, "%s", line);
    }

    // Press 'q' to quit
    while ((ch = wgetch(windowMain)) != 'q')
    {
        //! when u scroll down then go up, the old content gets deleted
        switch (ch)
        {
        case KEY_UP:
            // scroll up
            wscrl(windowMain, -1);
            break;
        case KEY_DOWN:
            // scroll down
            wscrl(windowMain, 1);
            break;
        }
        wrefresh(windowMain);
    }

    // wgetch(windowMain);
    // box(windowBorder, 0, 0);

    fclose(file);
    return ERR_NONE;
}

// returns ERR_NONE if the operation is successful
int file_delete(char pathSource[150])
{
    // todo delete file
    return ERR_NONE;
}