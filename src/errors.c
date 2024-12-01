#include "window_handler.h"
#include "errors.h"
#include "operation_handler.h"

#include <string.h>

_Bool is_empty(char *string)
{
    if (strcmp(string, "") == 0)
        return true;
    return false;
}

_Bool exit_input(char *string)
{
    if (strcmp(string, "") == 0 || strcmp(string, "exit") == 0 || strcmp(string, "") == 0)
    {
        destroy_window(windowMain);
        clear();
        noecho();
        refresh();
        return true;
    }
    return false;
}

// this function will print the error at (0, yPosition) if the error exists
void print_error(int yPosition, int error)
{
    if (error == ERR_NONE)
        return;
    if (error == ERR_FILE_NOT_FOUND)
    {
        mvwprintw(windowMain, yPosition, 0, "File not found.");
    }
    else if (error == ERR_FOLDER_NOT_FOUND)
    {
        mvwprintw(windowMain, yPosition, 0, "Folder not found.");
    }
    else if (error == ERR_PERMISSION_DENIED)
    {
        mvwprintw(windowMain, yPosition, 0, "Permission denied to access the folder.");
    }
    else if (error == ERR_UNKNOWN)
    {
        mvwprintw(windowMain, yPosition, 0, "Unknown error.");
    }
    else if (error == ERR_OUT_OF_BOUNDS)
    {
        mvwprintw(windowMain, yPosition, 0, "Line number is out of bounds.");
    }
    else if (error == ERR_TEXT_NOT_FOUND)
    {
        mvwprintw(windowMain, yPosition, 0, "Couldn't find the text to be replaced.");
    }
    wgetch(windowMain);
}
// this function will print the error at (0, yPosition)
void invalid_number_error(int yPosition)
{
    mvwprintw(windowMain, yPosition, 0, "please enter a valid number.");
    wgetch(windowMain);
    destroy_window(windowMain);

    // [11]
    clear();
    noecho();
    refresh();
    return;
}
