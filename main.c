#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>

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

    // *APP DECISION 1-4* in the word file
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
    // q for quitting
    while ((input = getch()) != 'q')
    {
        // returns the position of the selected item
        int currentIndex = item_index(current_item(menu));

        switch (input)
        {
        case KEY_DOWN:
            if (current_item == choicesLength - 1)
            {
                // If on the last item, wrap to the first item hehe
                menu_driver(menu, REQ_FIRST_ITEM);
                break;
            }
            // moves downards in the list
            menu_driver(menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            if (current_item == 0)
            {
                // If on the first item, wrap to the last item
                menu_driver(menu, REQ_LAST_ITEM);
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

            // TODO menu selection
            break;
        }
        refresh();
    }

    getch();
    endwin();
    return 0;
}
