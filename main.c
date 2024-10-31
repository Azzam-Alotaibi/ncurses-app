#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    // NULL is needed for new_menu() to work. and it's considered best practice
    const char *choices[] = {"Edit File", "Copy File", "Delete File", "Exit", NULL};

    // takes the size of the array and divide it by the first elements to get the full length without the NULL
    int choicesLength = sizeof(choices) / sizeof(choices[0]) - 1;

    // create an array pointer to an ITEM pointer and reserve the memory using calloc (contiguous allocation),
    // while casting the return type to a pointer to an item pointer
    // adds one cause of the NULL isn't calculated in the length
    ITEM **items = (ITEM **)calloc(choicesLength + 1, sizeof(ITEM *));

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

    // create the menu from the items array while casting the return type to a pointer to an item pointer
    MENU *menu = new_menu((ITEM **)items);

    // to show the menu to the screen
    post_menu(menu);

    // refresh the stdsc
    refresh();

    int input;
    bool isRunning = true;
    // this is responsible for the menu navigation using menu_driver()
    // q for quitting
    while (isRunning)
    {
        input = getch();
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
                mvprintw(5, 5, "Comeback later!");
            }
            // TODO menu selection

            break;
        case 'q':
            cleanup_menu(menu, items, choicesLength);
            isRunning = false;
        }
        refresh();
    }

    getch();
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