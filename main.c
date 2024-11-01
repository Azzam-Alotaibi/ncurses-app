#include "menu_handler.h"

#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>

// make a recursive function
int main()
{
    // The convention in C is has generally been to declare all such local variables at the top of a function [1]
    // *APP DECISIONS 5* in the word file
    WINDOW *windowMain;
    MENU *menu;
    ITEM **items;
    int choicesLength;
    bool isRunning;
    int input;

    // NULL is needed for new_menu() to work. and it's considered best practice
    const char *choices[] = {"Edit File", "Copy File", "Deledddte File", "Exit", NULL};

    // takes the size of the array and divide it by the first elements to get the full length without the NULL
    choicesLength = sizeof(choices) / sizeof(choices[0]) - 1;

    init_ncruses();

    // for setting up the menu, items. and we only send choices wihtout adressing the pointer because C automatically sends the first element's pointer
    setup_menu(&menu, &items, choicesLength, choices);

    // for setting up the window.
    setup_window(&windowMain, &menu, choicesLength);

    isRunning = true;
    // this is responsible for the menu navigation using menu_driver()
    // q for quitting
    // TODO to reduce redundancy, this while loop have to be extracted another function which accepts a function as an argument so it can be called in the case 10 section
    // TODO use a funciton pointer type to do that
    while (isRunning)
    {
        input = wgetch(windowMain);
        if (input == -1)
        {
            printw("error             ");
            getch();
            exit(0);
        }
        // returns the name of the selected item
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
                isRunning = false;
                cleanup_menu(menu, items, choicesLength);
                mvwprintw(windowMain, 5, 5, "Comeback later!");
                break;
            }
            cleanup_menu(menu, items, choicesLength);
            destroy_win(windowMain);
            if (strcmp(currentItemName, "Edit File") == 0)
            {
                edit_page(windowMain);
            }

            // for setting up the menu, items. and we only send choices wihtout adressing the pointer because C automatically sends the first element's pointer
            setup_menu(&menu, &items, choicesLength, choices);

            // for setting up the window.
            setup_window(&windowMain, &menu, choicesLength);

            // TODO menu selection

            break;
        case 'q':
            isRunning = false;
            cleanup_menu(menu, items, choicesLength);
            mvwprintw(windowMain, 5, 5, "Comeback later!");
            break;
        }
        wrefresh(windowMain);
    }

    wgetch(windowMain);

    destroy_win(windowMain);
    endwin();
    return 0;
}

void super_duper_recursion() {}

void edit_page(WINDOW *windowMain)
{
    // The convention in C is has generally been to declare all such local variables at the top of a function [1]
    MENU *menu;
    ITEM **items;
    int choicesLength;
    bool isRunning;
    int input;

    const char *choices[] = {"Append Line", "Insert Line", "Delete Line", "Show Line", "Go back", NULL};

    // takes the size of the array and divide it by the first elements to get the full length without the NULL
    choicesLength = sizeof(choices) / sizeof(choices[0]) - 1;

    // for setting up the menu, items. and we only send choices wihtout adressing the pointer because C automatically sends the first element's pointer
    setup_menu(&menu, &items, choicesLength, choices);

    // for setting up the window.
    setup_window(&windowMain, &menu, choicesLength);

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
            if (strcmp(currentItemName, "Go back") == 0)
            {
                cleanup_menu(menu, items, choicesLength);
                isRunning = false;
                break;
            }
            // TODO menu selection

            break;
        case 'q':
            cleanup_menu(menu, items, choicesLength);
            isRunning = false;
        }
        wrefresh(windowMain);
    }
}
