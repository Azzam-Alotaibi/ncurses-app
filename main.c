#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void cleanup_menu(MENU *menu, ITEM **items, int itemsLength);
void destroy_win(WINDOW *localWindow);

int main()
{
    // The convention in C is has generally been to declare all such local variables at the top of a function [1]
    WINDOW *windowMain;
    MENU *menu;
    ITEM **items;
    int startx, starty, height, width;
    int choicesLength;
    bool isRunning;
    int input;

    // NULL is needed for new_menu() to work. and it's considered best practice
    const char *choices[] = {"Edit File", "Copy File", "Delete File", "Exit", NULL};

    // takes the size of the array and divide it by the first elements to get the full length without the NULL
    choicesLength = sizeof(choices) / sizeof(choices[0]) - 1;

    // create an array pointer to an ITEM pointer and reserve the memory using calloc (contiguous allocation),
    // while casting the return type to a pointer to an item pointer
    // adds one cause of the NULL isn't calculated in the length
    items = (ITEM **)calloc(choicesLength + 1, sizeof(ITEM *));

    // create a new item out of every choice
    for (int i = 0; i < choicesLength; i++)
    {
        items[i] = new_item(choices[i], NULL);
    }
    // assign NULL to the end of the array for new_menu()
    items[choicesLength] = (ITEM *)NULL;

    // initializing the curses mode.
    // the cruses mode has to be initialized before calling the new_menu()
    initscr();

    // *APP DECISIONS 1-4* in the word file
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    height = 20;
    width = 50;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;
    windowMain = newwin(height, width, starty, startx);
    wrefresh(windowMain);
    box(windowMain, 0, 0);

    keypad(windowMain, TRUE);

    // create the menu from the items array while casting the return type to a pointer to an item pointer
    menu = new_menu((ITEM **)items);

    // set the main window for the menu
    set_menu_win(menu, windowMain);

    // create a sub-window for items and aling it within the box using derwin() [3]
    set_menu_sub(menu, derwin(windowMain, choicesLength, height - 1, 1, 2));

    // to show the menu to the screen
    post_menu(menu);

    // refresh the stdsc
    wrefresh(windowMain);

    isRunning = true;
    // this is responsible for the menu navigation using menu_driver()
    // q for quitting
    while (isRunning)
    {
        input = wgetch(windowMain);
        // returns the position of the selected item
        const char *currentItemName;
        currentItemName = item_name(current_item(menu));

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
        case 10: // 10 is the enter key for getch()
            if (strcmp(currentItemName, "Exit") == 0)
            {
                cleanup_menu(menu, items, choicesLength);
                isRunning = false;
                mvwprintw(windowMain, 5, 5, "Comeback later!");
            }
            // TODO menu selection

            break;
        case 'q':
            cleanup_menu(menu, items, choicesLength);
            isRunning = false;
        }
        wrefresh(windowMain);
    }

    wgetch(windowMain);
    destroy_win(windowMain);
    endwin();
    return 0;
}

// clean the menu to stop the memory leak
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
    // free the memory allocated/reserved by the array
    free(items);
}

void destroy_win(WINDOW *localWindow)
{
    /* box(local_win, ' ', ' '); : This won't produce the desired
     * result of erasing the window. It will leave its four corners
     * and so an ugly remnant of window. [2]
     */
    // so we have to use wborder()
    wborder(localWindow, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');

    wrefresh(localWindow);
    delwin(localWindow);
}