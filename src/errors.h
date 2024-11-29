// we wanted to use errno and change its value, since it treats FILE NOT FOUND and FOLDER NOT FOUND essentially the same.
// but turns out it's used under the hood a lot by different libraries and I/O operations.
// so we decided to make our own error values, and returning them directly in the operations method, it looks way cleaner than returning an araray of chars as an error.
// we made this implementation by reading the errno implementation.

// if not defined ERRORS_H define a new one, to pervent multiple instances of the same header file
// it's called "Include Guard"
#ifndef ERRORS_H
#define ERRORS_H

// Error codes
#define ERR_NONE 0
#define ERR_FILE_NOT_FOUND 1
#define ERR_FOLDER_NOT_FOUND 2
#define ERR_PERMISSION_DENIED 3
#define ERR_OUT_OF_BOUNDS 4
#define ERR_UNKNOWN 99

#endif
