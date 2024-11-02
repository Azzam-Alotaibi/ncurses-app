// menu.h
#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include <menu.h>

typedef bool (*EnterKeyHandler)(WINDOW *windowMain, int choicesLength, const char *currentItemName);

// function prototypes

// system operation
void clean_exit(WINDOW *windowMain);

// ncurses
void cleanup_menu(MENU *menu, ITEM **items, int itemsLength);
void destroy_win(WINDOW *localWindow);
void setup_menu(MENU **menu, ITEM ***items, int choicesLength, const char **choices);
void setup_window(WINDOW **windowMain, MENU **menu, int choicesLength);
void edit_page(WINDOW *windowMain);
void init_ncruses();

// pages
_Bool main_page(WINDOW *windowMain, int choicesLength, const char *currentItemName);
_Bool choose_file_page(WINDOW *windowMain, int choicesLength, const char *currentItemName);
_Bool edit_file_page(WINDOW *windowMain, int choicesLength, const char *currentItemName);
_Bool show_log_page(WINDOW *windowMain, int choicesLength, const char *currentItemName);

// layout
void super_duper_recursion(EnterKeyHandler enterKeyHandler, int choicesLength, const char **choices);

#endif
