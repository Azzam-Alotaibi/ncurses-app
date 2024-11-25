// if not defined ERRORS_H define a new one, to pervent multiple instances of the same header file
// it's called "Include Guard"
#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

// file manupilation functions
int file_copy(char pathSource[80], char pathDestination[80]);
int file_create(char pathSource[80]);
int file_show(char pathSource[80]);
int file_delete(char pathSource[80]);

// line manupilation functions

int line_append(char pathSource[80], int lineNumber);
int line_insert(char pathSource[80], int lineNumber);
int line_delete(char pathSource[80], int lineNumber);
int line_show(char pathSource[80], int lineNumber);

#endif