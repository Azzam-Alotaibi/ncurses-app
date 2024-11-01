#include "menu_handler.h"
#include <stdlib.h>

// initializing the curses mode with basic settings.
void init_ncruses()
{

    initscr();

    // *APP DECISIONS 1-4* in the word file
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

// clean the menu to prevent the memory leak
void cleanup_menu(MENU *menu, ITEM **items, int itemsLength)
{
    // hide the menu from the screen
    unpost_menu(menu);
    // free the memory allocated/reserved by the menu
    free_menu(menu);

    // free the memory allocated/reserved by every item
    for (int i = 0; i < itemsLength; ++i)
    {
        free_item(items[i]);
    }
    // free the memory allocated/reserved by the array which used malloc
    free(items);
}

void destroy_win(WINDOW *localWindow)
{
    delwin(localWindow);
    wrefresh(localWindow);
}

// *APP DECISIONS 6* in the word file
// this function sets up a new *MENU, and an array of *ITEM using the *choices array. It updates the variables using the pointers given so no need to return any value

void setup_menu(MENU **menu, ITEM ***items, int choicesLength, const char **choices)
{
    // create an array pointer to an ITEM pointer and reserve the memory using calloc (contiguous allocation),
    // while casting the return type to a pointer to an item pointer
    // adds one cause of the NULL isn't calculated in the length
    *items = (ITEM **)calloc(choicesLength + 1, sizeof(ITEM *));
    if (items == NULL)
    {
        perror("Failed to allocate memory for items");
        exit(EXIT_FAILURE);
    }

    // create a new item out of every choice
    for (int i = 0; i < choicesLength; i++)
    {
        (*items)[i] = new_item(choices[i], NULL);
        if (items[i] == NULL)
        {
            perror("Failed to create menu item");
            exit(EXIT_FAILURE);
        }
    }

    // assign NULL to the end of the array for new_menu()
    // we need *items to be between parantheses so we ensure the correct order of operations
    // without it, it won't consider *items an array and will access the [*choicesLength]th element of a single item
    (*items)[choicesLength] = (ITEM *)NULL;

    // crate a new meun using the items list
    *menu = new_menu(*items);
    if (menu == NULL)
    {
        perror("Failed to create menu");
        exit(EXIT_FAILURE);
    }
}

// *APP DECISIONS 7* in the word file
// this function sets up a new *WINDOW, using the *MENU provided in the argument. It updates the variables using the pointers given so no need to return any value
void setup_window(WINDOW **windowMain, MENU **menu, int choicesLength)
{

    // height and width in characters for the window
    int height = 20;
    int width = 50;

    // center the window
    // LINES and COLS are global variables provided by the library for the height and width of the stdscr (default screen)
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    // create a new window with the assigned attributes
    *windowMain = newwin(height, width, starty, startx);
    // create a box around the window as a border
    box(*windowMain, 0, 0);
    keypad(*windowMain, TRUE);

    // set the main window for the menu
    set_menu_win(*menu, *windowMain);

    // create a sub-window for items and aling it within the box using derwin() [3]
    set_menu_sub(*menu, derwin(*windowMain, choicesLength, height - 1, 1, 2));

    // to show the menu to the screen
    post_menu(*menu);
    wrefresh(*windowMain);
}
