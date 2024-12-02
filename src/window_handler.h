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
extern int HEIGHT;
extern int WIDTH_MENU;
extern int WIDTH_OPERATION;

// a funciton pointer to change the layout in each page
typedef bool (*EnterKeyHandler)(int optionsLength, const char *currentItemName);
typedef int (*OperationFileHandler)(char *pathSource);
typedef int (*OperationLineHandler)(char *pathSource, int lineNumber);

// function prototypes

// ncurses
void init_ncruses();
void clean_exit();
void cleanup_menu(MENU *menu, ITEM **items, int itemsLength);
void destroy_window(WINDOW *window);
void setup_menu(MENU **menu, ITEM ***items, int optionsLength, const char **options);
void setup_window_operation();
void setup_window_menu(MENU **menu, int optionsLength);
void edit_page();

// pages
_Bool main_page(int optionsLength, const char *currentItemName);
_Bool choose_file_page(int optionsLength, const char *currentItemName);
_Bool edit_file_page(int optionsLength, const char *currentItemName);

// layout
void super_duper_recursion(EnterKeyHandler enterKeyHandler, int optionsLength, const char **options);

// operation
void operation_file(char *operationName, OperationFileHandler operation);
void operation_file_copy();
void operation_line_UI(OperationLineHandler operationFunction);
void operation_line_append_UI();
void operation_line_insert_UI();
void text_replace_UI();
void file_encrypt_UI();

#endif
