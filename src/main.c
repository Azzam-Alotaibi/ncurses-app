#include "menu_handler.h"

#include <stdbool.h>
#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>

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
    setup_window_menu(&windowMain, &menu, choicesLength);

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
            destroy_win();

            isRunning = enterKeyHandler(choicesLength, currentItemName);

            // for setting up the menu, items. and we only send choices wihtout adressing the pointer because C automatically sends the first element's pointer
            setup_menu(&menu, &items, choicesLength, choices);

            // for setting up the window.
            setup_window_menu(&windowMain, &menu, choicesLength);
            break;
        case 'q':
            cleanup_menu(menu, items, choicesLength);
            clean_exit(windowMain);
            break;
        }
        wrefresh(windowMain);
    }
}
void operation_file(char *operationName)
{

    // ! old windowMain isn't deleted when creating this menu?
    setup_window(&windowMain);

    // [9]
    char str[80];
    // Move cursor to (1,1) and print prompt
    mvwprintw(windowMain, 1, 1, "Enter a string: ");
    echo();
    wgetstr(windowMain, str);
    noecho();
    // ! UI overflow bug
    mvwprintw(windowMain, 1, 2, "\nYou entered: %s\n", str);
    wrefresh(windowMain);
    wgetch(windowMain);
}