#include "menu_handler.h"
#include "operation_handler.h"
#include "errors.h"

#include <stdbool.h>
#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int main()
{

    int choicesLength;

    // NULL is needed for new_menu() to work. and it's considered best practice
    const char *choices[] = {"Choose File", "Create File", "Show Log", "Exit", NULL};

    // takes the size of the array and divide it by the first elements to get the full length without the NULL
    choicesLength = sizeof(choices) / sizeof(choices[0]) - 1;

    init_ncruses();

    // setting the function pointer to the main page functionality
    EnterKeyHandler main_pg = main_page;
    super_duper_recursion(main_pg, choicesLength, choices);
}

void super_duper_recursion(EnterKeyHandler enterKeyHandler, int choicesLength, const char **choices)
{
    // The convention in C is has generally been to declare all such local variables at the top of a function [1]
    // *APP DECISIONS 5* in the word file
    MENU *menu;
    ITEM **items;
    bool isRunning;
    int input;

    /*for setting up the &menu and &items.
     * also, we only send choices wihtout adressing the pointer because C automatically sends the first element's pointer of the array
     */
    // [6]
    setup_menu(&menu, &items, choicesLength, choices);

    // for setting up the window.
    setup_window_menu(&menu, choicesLength);

    isRunning = true;
    // this is responsible for the menu navigation using menu_driver()
    // q for quitting

    while (isRunning)
    {
        input = wgetch(windowMain);
        // returns the position of the selected item
        const char *currentItemName = item_name(current_item(menu));

        switch (input)
        {
        case KEY_DOWN:
            if (strcmp(currentItemName, choices[choicesLength - 1]) == 0)
            {
                // If on the last item, wrap to the first item hehe
                menu_driver(menu, REQ_FIRST_ITEM);
                break;
            }
            // moves downards in the list
            menu_driver(menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            if (strcmp(currentItemName, choices[0]) == 0)
            {
                // If on the first item, wrap to the last item
                menu_driver(menu, REQ_LAST_ITEM);
                break;
            }
            // moves upwards in the list
            menu_driver(menu, REQ_UP_ITEM);
            break;
        case KEY_RIGHT:
            // moves to the last item
            menu_driver(menu, REQ_LAST_ITEM);
            break;
        case KEY_LEFT:
            // moves to the first item
            menu_driver(menu, REQ_FIRST_ITEM);
            break;
        case 10: // 10 is the enter key for wgetch() / getch()
            cleanup_menu(menu, items, choicesLength);
            destroy_window(windowMain);

            isRunning = enterKeyHandler(choicesLength, currentItemName);

            // for setting up the menu, items. and we only send choices wihtout adressing the pointer because C automatically sends the first element's pointer
            setup_menu(&menu, &items, choicesLength, choices);

            // for setting up the window.
            setup_window_menu(&menu, choicesLength);
            break;
        case 'q':
            cleanup_menu(menu, items, choicesLength);
            clean_exit();
            break;
        }
        wrefresh(windowMain);
    }
}

// for duplicating, deleting, creating
void operation_file(char *operationName, OperationFileHandler operation)
{
    char message[100];
    int yPosition = 0, error;
    char pathSource[150];

    setup_window_operation(0);
    // [9]
    echo();

    // formatting the message string
    // for some reason, in macos the app needs to specify the message size otherwise it just crashes
    snprintf(message, sizeof(message), "Please enter the file's path to %s file. like this format /home/etc/file.txt \npath: ", operationName);

    // we used a string literal to avoid Wformat-security [12]
    // print prompt the following prompt at the piont (0, 0)
    mvwprintw(windowMain, yPosition, 0, "%s", message);

    // enabling echo to make the user type more than one charecter
    // passing the str pointer to make the function assign the user inputs to the str
    wgetstr(windowMain, pathSource);

    // calculate how many lines is the input and add that to the y position
    yPosition = ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 3;

    // TODO handle the output and errors

    error = operation(pathSource);
    if (error == ERR_FILE_NOT_FOUND)
    {
        mvwprintw(windowMain, yPosition, 0, "File not found.");
    }
    // mvwprintw(windowMain, yPosition, 0, "You entered: %s\n", pathSource);

    destroy_window(windowMain);

    // some text outside the new window isn't getting cleared when the app delete the window. This clear(); call fixes it.
    // [11]
    clear();
    noecho();
    refresh();
}

void operation_file_copy()
{
    char pathSource[150];
    char pathDestination[150];
    int yPosition = 0;
    int error;
    const char *messageFirst = "Please enter the file's path to copy it like this /home/etc/file.txt \npath: ";
    const char *messageSecond = "Please enter the destination path and the file name like this /home/etc/newFile.txt \npath: ";

    setup_window_operation(0);
    // enabling echo to make the user type more than one charecter
    // [9]
    echo();
    // we used a string literal to avoid Wformat-security [12]
    // print prompt the following prompt at the piont (0, 0)
    mvwprintw(windowMain, yPosition, 0, "%s", messageFirst);
    // passing the str pointer to make the function assign the user inputs to the str
    wgetstr(windowMain, pathSource);

    // calculate how many lines is the input and add that to the y position
    yPosition = ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 3;

    mvwprintw(windowMain, yPosition, 0, "%s", messageSecond);
    wgetstr(windowMain, pathDestination);

    yPosition += ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 3;

    // TODO handle the output and errors
    error = file_copy(pathSource, pathDestination);
    if (error == ERR_FILE_NOT_FOUND)
    {
        mvwprintw(windowMain, yPosition, 0, "File not found.");
    }
    wrefresh(windowMain);
    wgetch(windowMain);

    destroy_window(windowMain);

    // some text outside the new window isn't getting cleared when the app delete the window. This clear(); call fixes it.
    // [11]
    clear();
    noecho();
    refresh();
}

void operation_line(char *operationName, OperationLineHandler operationFunction)
{
    char messageFirst[85];
    char pathSource[150];
    char lineNumberchar[11];
    char *endptr;
    int lineNumber;
    int error;
    char messageSecond[] = "Please enter the line number: ";
    int yPosition = 0;

    setup_window_operation(0);
    // [9]
    echo();

    // formatting the message string
    sprintf(messageFirst, "Please enter the file's path to %s, like this /home/etc/file.txt \npath: ", operationName);

    // we used a string literal to avoid Wformat-security [12]
    // print prompt the following prompt at the piont (0, 0)
    mvwprintw(windowMain, yPosition, 0, "%s", messageFirst);
    // passing the str pointer to make the function assign the user inputs to the str
    wgetstr(windowMain, pathSource);

    // calculate how many lines is the input and add that to the y position
    yPosition = ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 3;

    mvwprintw(windowMain, yPosition, 0, "%s", messageSecond);
    wgetstr(windowMain, lineNumberchar);
    // [13]
    lineNumber = strtol(lineNumberchar, &endptr, 10);
    if (*endptr == '\0')
    {
        // TODO handle the error
    }

    yPosition += ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 2;

    // TODO handle the output and errors

    error = operationFunction(pathSource, lineNumber);
    if (error == ERR_FILE_NOT_FOUND)
    {
        mvwprintw(windowMain, yPosition, 0, "File not found.");
    }
    wrefresh(windowMain);
    wgetch(windowMain);

    destroy_window(windowMain);

    // some text outside the new window isn't getting cleared when the app delete the window. This clear(); call fixes it.
    // [11]
    clear();
    noecho();
    refresh();
}