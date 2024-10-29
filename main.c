#include <ncurses.h>

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int row, col;
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2, (col - 12) / 2, "Hello, World!");
    refresh();
    getch();
    endwin();
    return 0;
}
