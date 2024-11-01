// menu.h
#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include <menu.h>

typedef void (*EnterKeyHandler)(MENU *menu, ITEM **items, int choicesLength, WINDOW *windowMain, const char **choices);

// Function prototypes
void cleanup_menu(MENU *menu, ITEM **items, int itemsLength);
void destroy_win(WINDOW *localWindow);
void setup_menu(MENU **menu, ITEM ***items, int choicesLength, const char **choices);
void setup_window(WINDOW **windowMain, MENU **menu, int choicesLength);
void edit_page(WINDOW *windowMain);
void init_ncruses();

#endif
