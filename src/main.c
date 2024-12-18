#include "window_handler.h"
#include "operation_handler.h"
#include "errors.h"
#include <stdbool.h>
#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{

    int optionsLength;
    const int MINIMUM_HIEGHT = 26;

    // NULL is needed for new_menu() to work. and it's considered best practice
    const char *options[] = {"Choose File", "Create File", "Show Log", "Exit", NULL};

    // takes the size of the array and divide it by the first elements to get the full length without the NULL
    optionsLength = sizeof(options) / sizeof(options[0]) - 1;

    init_ncruses();
    if (LINES < MINIMUM_HIEGHT)
    {
        endwin();
        printf("Your window's height  is too narrow, sorry you can't open the app.\n");
        exit(0);
    }

    // setting the function pointer to the main page functionality
    EnterKeyHandler main_pg = main_page;
    super_duper_recursion(main_pg, optionsLength, options);
}

void super_duper_recursion(EnterKeyHandler enterKeyHandler, int optionsLength, const char **options)
{
    // The convention in C is has generally been to declare all such local variables at the top of a function [1]
    // *APP DECISIONS 5* in the word file
    MENU *menu;
    ITEM **items;
    bool isRunning;
    int input;

    /*for setting up the &menu and &items.
     * also, we only send options wihtout adressing the pointer because C automatically sends the first element's pointer of the array
     */
    // [6]
    setup_menu(&menu, &items, optionsLength, options);

    // for setting up the window.
    setup_window_menu(&menu, optionsLength);

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
            if (strcmp(currentItemName, options[optionsLength - 1]) == 0)
            {
                // If on the last item, wrap to the first item hehe
                menu_driver(menu, REQ_FIRST_ITEM);
                break;
            }
            // moves downards in the list
            menu_driver(menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            if (strcmp(currentItemName, options[0]) == 0)
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
            cleanup_menu(menu, items, optionsLength);
            destroy_window(windowMain);

            isRunning = enterKeyHandler(optionsLength, currentItemName);

            // for setting up the menu, items. and we only send options wihtout adressing the pointer because C automatically sends the first element's pointer
            setup_menu(&menu, &items, optionsLength, options);

            // for setting up the window.
            setup_window_menu(&menu, optionsLength);
            break;
        case 'q':
            cleanup_menu(menu, items, optionsLength);
            destroy_window(windowMain);
            clean_exit();
            break;
        }
        wrefresh(windowMain);
    }
}

// for duplicating, deleting, creating
void operation_file(char *operationName, OperationFileHandler operation)
{
    char message[110];
    int yPosition = 0, error;
    char pathSource[150];

    setup_window_operation();
    // [9]
    echo();

    // formatting the message string
    // for some reason, in MacOS the app needs to specify the message size otherwise it just crashes
    snprintf(message, sizeof(message), "Please enter the file's path to %s file. like this format /home/etc/file.txt \npath: ", operationName);

    // we used a string literal to avoid Wformat-security [12]
    // print prompt the following prompt at the piont (0, 0)
    mvwprintw(windowMain, yPosition, 0, "%s", message);

    // enabling echo to make the user type more than one charecter
    // passing the str pointer to make the function assign the user inputs to the str
    wgetstr(windowMain, pathSource);

    // handle exits
    if (exit_input(pathSource))
        return;

    // calculate how many lines is the input and add that to the y position
    yPosition += ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 4;

    error = operation(pathSource);
    print_error(yPosition, error);

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

    setup_window_operation();
    // enabling echo to make the user type more than one charecter
    // [9]
    echo();
    // we used a string literal to avoid Wformat-security [12]
    // print prompt the following prompt at the piont (0, 0)
    mvwprintw(windowMain, yPosition, 0, "%s", messageFirst);
    // passing the str pointer to make the function assign the user inputs to the str
    wgetstr(windowMain, pathSource);

    // handle exits
    if (exit_input(pathSource))
        return;

    // calculate how many lines is the input and add that to the y position
    yPosition += ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 3;

    mvwprintw(windowMain, yPosition, 0, "%s", messageSecond);
    wgetstr(windowMain, pathDestination);

    // handle exits
    if (exit_input(pathDestination))
        return;

    yPosition += ceil((strlen(pathDestination) + 6) / WIDTH_OPERATION) + 3;

    error = file_copy(pathSource, pathDestination);
    print_error(yPosition, error);

    destroy_window(windowMain);

    // some text outside the new window isn't getting cleared when the app delete the window. This clear(); call fixes it.
    // [11]
    clear();
    noecho();
    refresh();
}

void operation_line_append_UI()
{
    char pathSource[150];
    char newLine[513];
    int yPosition = 0;
    int error;
    const char *messageFirst = "Please enter the file's path like this /home/etc/file.txt, if the file doesn't exist, it will create a new file \npath: ";
    const char *messageSecond = "Please enter the new line you'd like to add to the file (max 512 characters) \nNew Line: ";

    setup_window_operation();
    // enabling echo to make the user type more than one charecter
    // [9]
    echo();
    // we used a string literal to avoid Wformat-security [12]
    // print prompt the following prompt at the piont (0, 0)
    mvwprintw(windowMain, yPosition, 0, "%s", messageFirst);
    // passing the str pointer to make the function assign the user inputs to the str
    wgetstr(windowMain, pathSource);

    // handle exits
    if (exit_input(pathSource))
        return;

    // calculate how many lines is the input and add that to the y position
    yPosition += ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 4;

    mvwprintw(windowMain, yPosition, 0, "%s", messageSecond);
    wgetstr(windowMain, newLine);

    // handle exits
    if (exit_input(newLine))
        return;

    yPosition += ceil((strlen(newLine) + 10) / WIDTH_OPERATION) + 3;

    error = line_append(pathSource, newLine);
    print_error(yPosition, error);

    destroy_window(windowMain);

    // some text outside the new window isn't getting cleared when the app delete the window. This clear(); call fixes it.
    // [11]
    clear();
    noecho();
    refresh();
}

void operation_line_insert_UI()
{
    char pathSource[150], newLine[513], lineNumberString[10];
    char *endptr;
    int yPosition = 0, error, lineNumber;
    const char *messageFirst = "Please enter the file's path like this /home/etc/file.txt \npath: ";
    const char *messageSecond = "Please enter the new line you'd like to add to the file (max 512 characters) \nNew Line: ";
    const char *messageThird = "Please enter the line number you'd like to insert the line just under \nLine Number: ";

    setup_window_operation();
    // enabling echo to make the user type more than one charecter
    // [9]
    echo();
    // we used a string literal to avoid Wformat-security [12]
    // print prompt the following prompt at the piont (0, 0)
    mvwprintw(windowMain, yPosition, 0, "%s", messageFirst);
    // passing the str pointer to make the function assign the user inputs to the str
    wgetstr(windowMain, pathSource);

    // handle exits
    if (exit_input(pathSource))
        return;

    // calculate how many lines is the input and add that to the y position
    yPosition += ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 3;

    mvwprintw(windowMain, yPosition, 0, "%s", messageSecond);
    wgetstr(windowMain, newLine);

    // handle exits
    if (exit_input(newLine))
        return;

    yPosition += ceil((strlen(newLine) + 10) / WIDTH_OPERATION) + 3;

    mvwprintw(windowMain, yPosition, 0, "%s", messageThird);
    wgetstr(windowMain, lineNumberString);

    // handle exits
    if (exit_input(lineNumberString))
        return;

    yPosition += ceil((strlen(lineNumberString) + 14) / WIDTH_OPERATION) + 3;

    lineNumber = strtol(lineNumberString, &endptr, 10);
    if (*endptr != '\0' || lineNumber == 0)
    {
        invalid_number_error(yPosition);
        return;
    }

    error = line_insert(pathSource, lineNumber, newLine);
    print_error(yPosition, error);

    destroy_window(windowMain);

    // some text outside the new window isn't getting cleared when the app delete the window. This clear(); call fixes it.
    // [11]
    clear();
    noecho();
    refresh();
}

void operation_line_UI(OperationLineHandler operationFunction)
{
    char pathSource[150], lineNumberString[10];
    char *endptr;
    int lineNumber, error, yPosition = 0;
    const char *messageFirst = "Please enter the file's path like this /home/etc/file.txt \npath: ";
    const char *messageSecond = "Please enter the line number: ";

    setup_window_operation();
    // [9]
    echo();

    // formatting the message string

    // we used a string literal to avoid Wformat-security [12]
    // print prompt the following prompt at the piont (0, 0)
    mvwprintw(windowMain, yPosition, 0, "%s", messageFirst);
    // passing the str pointer to make the function assign the user inputs to the str
    wgetstr(windowMain, pathSource);

    // handle exits
    if (exit_input(pathSource))
        return;

    // calculate how many lines is the input and add that to the y position
    yPosition += ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 3;

    mvwprintw(windowMain, yPosition, 0, "%s", messageSecond);
    wgetstr(windowMain, lineNumberString);

    // handle exits
    if (exit_input(lineNumberString))
        return;

    yPosition += ceil((strlen(lineNumberString) + 6) / WIDTH_OPERATION) + 2;

    // [13]
    lineNumber = strtol(lineNumberString, &endptr, 10);
    if (*endptr != '\0' || lineNumber == 0)
    {
        invalid_number_error(yPosition);
        return;
    }

    error = operationFunction(pathSource, lineNumber);
    print_error(yPosition, error);

    destroy_window(windowMain);

    // some text outside the new window isn't getting cleared when the app delete the window. This clear(); call fixes it.
    // [11]
    clear();
    noecho();
    refresh();
}

void log_operation(char *line)
{
    time_t timeCurrent;
    FILE *file;

    timeCurrent = time(0);
    char *timeString = ctime(&timeCurrent);
    file = fopen("logs.txt", "a");
    strtok(timeString, "\n");
    fprintf(file, "%s:%s\n", timeString, line);
    fclose(file);
}

void text_replace_UI()
{
    char pathSource[150], textOriginal[513], textNew[513];
    char *endptr;
    int lineNumber, error, yPosition = 0;
    const char *messageFirst = "Please enter the file's path like this /home/etc/file.txt \npath: ";
    const char *messageSecond = "Please enter the text you'd like to replace (max 512 characters): ";
    const char *messageThird = "Please enter the new text (max 512 characters): ";

    setup_window_operation();
    // [9]
    echo();

    // formatting the message string

    // we used a string literal to avoid Wformat-security [12]
    // print prompt the following prompt at the piont (0, 0)
    mvwprintw(windowMain, yPosition, 0, "%s", messageFirst);
    // passing the str pointer to make the function assign the user inputs to the str
    wgetstr(windowMain, pathSource);

    // handle exits
    if (exit_input(pathSource))
        return;

    // calculate how many lines is the input and add that to the y position
    yPosition += ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 3;

    mvwprintw(windowMain, yPosition, 0, "%s", messageSecond);
    wgetstr(windowMain, textOriginal);

    // handle exits
    if (exit_input(textOriginal))
        return;

    yPosition += ceil((strlen(textOriginal) + strlen(messageSecond)) / WIDTH_OPERATION) + 3;

    mvwprintw(windowMain, yPosition, 0, "%s", messageThird);
    wgetstr(windowMain, textNew);

    // no handle exit here because don't wanna ignore the user input

    yPosition += ceil((strlen(textNew) + strlen(messageThird)) / WIDTH_OPERATION) + 3;

    error = text_replace(pathSource, textOriginal, textNew);
    print_error(yPosition, error);

    destroy_window(windowMain);

    // some text outside the new window isn't getting cleared when the app delete the window. This clear(); call fixes it.
    // [11]
    clear();
    noecho();
    refresh();
}

void file_encrypt_UI()
{
    char pathSource[150], key[513];
    char *endptr;
    int lineNumber, error, yPosition = 0;
    const char *messageFirst = "Please enter the file's path to encrypt or decrypt it, the path should look like this /home/etc/file.txt \npath: ";
    const char *messageSecond = "Please enter the key to encrypt/decrypt it (don't forget it!) (max 512 characters) \nkey: ";

    setup_window_operation();
    // [9]
    echo();

    // formatting the message string

    // we used a string literal to avoid Wformat-security [12]
    // print prompt the following prompt at the piont (0, 0)
    mvwprintw(windowMain, yPosition, 0, "%s", messageFirst);
    // passing the str pointer to make the function assign the user inputs to the str
    wgetstr(windowMain, pathSource);

    // handle exits
    if (exit_input(pathSource))
        return;

    // calculate how many lines is the input and add that to the y position
    yPosition += ceil((strlen(pathSource) + 6) / WIDTH_OPERATION) + 5;

    mvwprintw(windowMain, yPosition, 0, "%s", messageSecond);
    wgetstr(windowMain, key);

    // handle exits
    if (exit_input(key))
        return;

    yPosition += ceil((strlen(key) + 6) / WIDTH_OPERATION) + 3;

    error = file_encrypt(pathSource, key);
    print_error(yPosition, error);

    destroy_window(windowMain);

    // some text outside the new window isn't getting cleared when the app delete the window. This clear(); call fixes it.
    // [11]
    clear();
    noecho();
    refresh();
}
