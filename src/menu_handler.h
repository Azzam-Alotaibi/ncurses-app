// if not defined MENU_H define a new one, to pervent multiple instances of the same header file
// it's called "Include Guard"
#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include <menu.h>

// global variables
// we decided to let windowMain a global variable since it's literally used across the app and passing it on ever function call doesn't make sense anymore.
extern WINDOW *windowMain;

// a funciton pointer to change the layout in each page
typedef bool (*EnterKeyHandler)(int choicesLength, const char *currentItemName);

// function prototypes

// ncurses
void init_ncruses();
void clean_exit();
void cleanup_menu(MENU *menu, ITEM **items, int itemsLength);
void destroy_win();
void setup_menu(MENU **menu, ITEM ***items, int choicesLength, const char **choices);
void setup_window(WINDOW **windowMain);
void setup_window_menu(WINDOW **windowMain, MENU **menu, int choicesLength);
void edit_page();

// pages
_Bool main_page(int choicesLength, const char *currentItemName);
_Bool choose_file_page(int choicesLength, const char *currentItemName);
_Bool edit_file_page(int choicesLength, const char *currentItemName);
_Bool show_log_page(int choicesLength, const char *currentItemName);

// layout
void super_duper_recursion(EnterKeyHandler enterKeyHandler, int choicesLength, const char **choices);

// operation
void operation_file(char *operationName);

#endif
