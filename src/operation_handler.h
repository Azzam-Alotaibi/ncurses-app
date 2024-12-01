// if not defined ERRORS_H define a new one, to pervent multiple instances of the same header file
// it's called "Include Guard"
#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

// file manupilation functions
int file_copy(char *pathSource, char *pathDestination);
int file_create(char *pathSource);
int file_show(char *pathSource);
int file_delete(char *pathSource);
int file_count_lines(char *pathSource);
int does_file_exist(FILE *file, char mode[1]);

// text manupilation functions
int line_append(char *pathSource, char *newLine);
int line_insert(char *pathSource, int lineNumber, char *newLine);
int line_delete(char *pathSource, int lineNumber);
int line_show(char *pathSource, int lineNumber);
int text_replace(char *pathSource, char *textOriginal, char *textReplace);

// for loging
void log_operation(char *line);
int file_count_lines_without_window(char *pathSource);

#endif