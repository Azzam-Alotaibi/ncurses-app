// if not defined MENU_H define a new one, to pervent multiple instances of the same header file
// it's called "Include Guard"
#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include <menu.h>

// global variables
// we decided to let windowMain a global variable since it's literally used across the app and passing it on ever function call doesn't make sense anymore.
extern WINDOW *windowMain;
extern WINDOW *windowBorder;
extern const int HEIGHT_MENU;
extern const int WIDTH_MENU;
extern const int HEIGHT_OPERATION;
extern const int WIDTH_OPERATION;

// a funciton pointer to change the layout in each page
typedef bool (*EnterKeyHandler)(int choicesLength, const char *currentItemName);
typedef int (*OperationFileHandler)(char pathSource[150]);
typedef int (*OperationLineHandler)(char pathSource[150], int lineNumber);

// function prototypes

// ncurses
void init_ncruses();
void clean_exit();
void cleanup_menu(MENU *menu, ITEM **items, int itemsLength);
void destroy_win();
void setup_menu(MENU **menu, ITEM ***items, int choicesLength, const char **choices);
void setup_window_operation();
void setup_window_menu(MENU **menu, int choicesLength);
void edit_page();

// pages
_Bool main_page(int choicesLength, const char *currentItemName);
_Bool choose_file_page(int choicesLength, const char *currentItemName);
_Bool edit_file_page(int choicesLength, const char *currentItemName);
_Bool show_log_page(int choicesLength, const char *currentItemName);

// layout
void super_duper_recursion(EnterKeyHandler enterKeyHandler, int choicesLength, const char **choices);

// operation
void operation_file(char *operationName, OperationFileHandler operation);
void operation_file_copy();
void operation_line(char *operationName, OperationLineHandler operationFunction);

#endif
