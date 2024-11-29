// if not defined ERRORS_H define a new one, to pervent multiple instances of the same header file
// it's called "Include Guard"
#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

// file manupilation functions
int file_copy(char pathSource[150], char pathDestination[150]);
int file_create(char pathSource[150]);
int file_show(char pathSource[150]);
int file_delete(char pathSource[150]);
int does_file_exist(FILE *file, char mode[1]);

// line manupilation functions

int line_append(char pathSource[150], const char *newLine);
int line_insert(char pathSource[150], int lineNumber, const char *newLine);
int line_delete(char pathSource[150], int lineNumber);
int line_show(char pathSource[150], int lineNumber);

#endif