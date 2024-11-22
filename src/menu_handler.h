// if not defined MENU_H define a new one, to pervent multiple instances of the same header file
// it's called "Include Guard"
#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include <menu.h>

// a funciton pointer to change the layout in each page
typedef bool (*EnterKeyHandler)(WINDOW *windowMain, int choicesLength, const char *currentItemName);

// function prototypes

// ncurses
void init_ncruses();
void clean_exit(WINDOW *windowMain);
void cleanup_menu(MENU *menu, ITEM **items, int itemsLength);
void destroy_win(WINDOW *localWindow);
void setup_menu(MENU **menu, ITEM ***items, int choicesLength, const char **choices);
void setup_window(WINDOW **windowMain, MENU **menu, int choicesLength);
void edit_page(WINDOW *windowMain);

// pages
_Bool main_page(WINDOW *windowMain, int choicesLength, const char *currentItemName);
_Bool choose_file_page(WINDOW *windowMain, int choicesLength, const char *currentItemName);
_Bool edit_file_page(WINDOW *windowMain, int choicesLength, const char *currentItemName);
_Bool show_log_page(WINDOW *windowMain, int choicesLength, const char *currentItemName);

// layout
void super_duper_recursion(EnterKeyHandler enterKeyHandler, int choicesLength, const char **choices);

#endif
