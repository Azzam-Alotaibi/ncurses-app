#include "window_handler.h"
#include "errors.h"
#include "operation_handler.h"

#include <stdlib.h>
#include <string.h>

// global variable
WINDOW *windowMain;
WINDOW *windowBorder;
int HEIGHT;
int WIDTH_MENU;
int WIDTH_OPERATION;

// initializing the curses mode with basic settings.
void init_ncruses()
{

    initscr();

    // *APP DECISIONS 1-4* in the word file
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // LINES and COLS are global variables provided by the library for the height and width of the stdscr (default screen)
    // 13 48
    HEIGHT = LINES * .5;
    WIDTH_MENU = COLS * .5;
}

// clean the menu to prevent the memory leak
void cleanup_menu(MENU *menu, ITEM **items, int itemsLength)
{
    // hide the menu from the screen [5]
    unpost_menu(menu);
    // free the memory allocated/reserved by the menu [5]
    free_menu(menu);

    // free the memory allocated/reserved by every item [5]
    for (int i = 0; i < itemsLength; ++i)
    {
        free_item(items[i]);
    }
    // free the memory allocated/reserved by the array which used calloc
    free(items);
}

void destroy_window(WINDOW *window)
{
    // [7]
    wborder(window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(window);
    delwin(window);
}

// *APP DECISIONS 6* in the word file
// this function sets up a new *MENU, and an array of *ITEM using the *options array. It updates the variables using the pointers given so no need to return any value

void setup_menu(MENU **menu, ITEM ***items, int optionsLength, const char **options)
{

    *items = (ITEM **)calloc(optionsLength + 1, sizeof(ITEM *));
    if (items == NULL)
    {
        perror("Failed to allocate memory for items");
        exit(EXIT_FAILURE);
    }

    // create a new item out of every choice
    for (int i = 0; i < optionsLength; i++)
    {
        // create an item in the ith element
        // the null is passed here since we don't need a description for our items
        (*items)[i] = new_item(options[i], NULL);
        if ((*items)[i] == NULL)
        {
            perror("Failed to create menu item");
            exit(EXIT_FAILURE);
        }
    }

    // assign NULL to the end of the array so the new_menu() knows where to stop.
    (*items)[optionsLength] = NULL;

    // crate a new meun using the items list
    *menu = new_menu(*items);
    if (*menu == NULL)
    {
        perror("Failed to create menu");
        exit(EXIT_FAILURE);
    }
}

// *APP DECISIONS 7* in the word file
// this function sets up a new *WINDOW, using the *MENU provided in the argument. It updates the variables using the pointers given so no need to return any value
void setup_window_menu(MENU **menu, int optionsLength)
{

    // center the window

    int starty = (LINES - HEIGHT) / 2;
    int startx = (COLS - WIDTH_MENU) / 2;

    // create a new window with the assigned attributes
    windowMain = newwin(HEIGHT, WIDTH_MENU, starty, startx);
    // create a box around the window as a border
    box(windowMain, 0, 0);
    keypad(windowMain, TRUE);

    // set the main window for the menu
    set_menu_win(*menu, windowMain);

    // create a sub-window for items and aling it within the box using derwin() [3]
    set_menu_sub(*menu, derwin(windowMain, optionsLength, HEIGHT - 1, 1, 2));

    // to show the menu to the screen
    post_menu(*menu);
    wrefresh(windowMain);
}

void setup_window_operation()
{
    WIDTH_OPERATION = COLS * 0.8;
    // center the window
    // LINES and COLS are global variables provided by the library for the height and width of the stdscr (default screen)
    const int starty = (LINES - HEIGHT) / 2;
    const int startx = (COLS - WIDTH_OPERATION) / 2;

    // the app will always override the borders by the user inputs, in order to solve that this window is created with
    // a +2 pixels padding in horizontally and +1 pixel vertically.
    // we did this so the borders are in completely different window to avoid the overriding of the user input.
    // windowBorder = newwin(HEIGHT, WIDTH_OPERATION, starty, startx);

    windowBorder = newwin(HEIGHT + 2, WIDTH_OPERATION + 4, starty - 1, startx - 2);
    box(windowBorder, 0, 0);
    wrefresh(windowBorder);

    // create a new window with the assigned attributes
    windowMain = newwin(HEIGHT, WIDTH_OPERATION, starty, startx);
    // create a box around the window as a border
    // box(windowMain, 0, 0);
    keypad(windowMain, TRUE);

    // enable scroling to show the big files
    scrollok(windowMain, TRUE);
    scrollok(windowBorder, TRUE);

    wrefresh(windowMain);
}

void clean_exit()
{
    char message[] = "Comeback Later!";

    // center the message
    mvprintw(LINES / 2, (COLS - strlen(message)) / 2, "%s", message);
    getch();
    endwin();
    exit(0);
}

// pages
_Bool main_page(int optionsLength, const char *currentItemName)
{
    if (strcmp(currentItemName, "Exit") == 0)
    {
        clean_exit();
    }
    if (strcmp(currentItemName, "Choose File") == 0)
    {
        const char *options[] = {"Edit File", "Copy File", "Delete File", "Show File", "Line Count", "Encrypt File",
                                 "Go Back",
                                 NULL};

        // takes the size of the array and divide it by the first elements to get the full length without the NULL
        optionsLength = sizeof(options) / sizeof(options[0]) - 1;

        // [8]
        EnterKeyHandler function = choose_file_page;
        super_duper_recursion(function, optionsLength, options);
    }
    else if (strcmp(currentItemName, "Create File") == 0)
    {
        OperationFileHandler operation = file_create;
        operation_file("create", operation);
    }
    else if (strcmp(currentItemName, "Show Log") == 0)
    {
        int error;

        setup_window_operation();

        error = file_show("logs.txt");
        print_error(0, error);

        // mvwprintw(windowMain, yPosition, 0, "You entered: %s\n", pathSource);

        destroy_window(windowMain);

        // some text outside the new window isn't getting cleared when the app delete the window. This clear(); call fixes it.
        // [11]
        clear();
        refresh();
    }
    return true;
}

_Bool choose_file_page(int optionsLength, const char *currentItemName)
{
    if (strcmp(currentItemName, "Go Back") == 0)
    {
        return false;
    }
    if (strcmp(currentItemName, "Edit File") == 0)
    {
        const char *options[] = {"Append Line", "Insert Line", "Delete Line", "Show Line", "Replace Text", "Go Back", NULL};

        // takes the size of the array and divide it by the first elements to get the full length without the NULL
        optionsLength = sizeof(options) / sizeof(options[0]) - 1;

        // [8]
        EnterKeyHandler function = edit_file_page;
        super_duper_recursion(function, optionsLength, options);
    }
    else if (strcmp(currentItemName, "Copy File") == 0)
    {
        operation_file_copy();
    }
    else if (strcmp(currentItemName, "Delete File") == 0)
    {
        OperationFileHandler operation = file_delete;
        operation_file("delete", operation);
    }
    else if (strcmp(currentItemName, "Show File") == 0)
    {
        OperationFileHandler operation = file_show;
        operation_file("show", operation);
    }
    else if (strcmp(currentItemName, "Line Count") == 0)
    {
        OperationFileHandler operation = file_count_lines;
        operation_file("count the lines in a", operation);
    }
    else if (strcmp(currentItemName, "Encrypt File") == 0)
    {
        file_encrypt_UI();
    }
    return true;
}

_Bool edit_file_page(int optionsLength, const char *currentItemName)
{
    if (strcmp(currentItemName, "Go Back") == 0)
    {
        return false;
    }
    if (strcmp(currentItemName, "Append Line") == 0)
    {
        operation_line_append_UI();
    }
    else if (strcmp(currentItemName, "Insert Line") == 0)
    {
        operation_line_insert_UI();
    }
    else if (strcmp(currentItemName, "Delete Line") == 0)
    {
        OperationLineHandler operation = line_delete;
        operation_line_UI(operation);
    }
    else if (strcmp(currentItemName, "Replace Text") == 0)
    {
        text_replace_UI();
    }
    else if (strcmp(currentItemName, "Show Line") == 0)
    {
        OperationLineHandler operation = line_show;
        operation_line_UI(operation);
    }
    return true;
}