// gcc -o curses -Wall -L/usr/lib -lncurses curses.c
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int x = 0;
    int y = 0;
    char thing = 'X';
    bool pen_down = false;
    bool done = false;
    while (!done) {
        if (!pen_down)
            clear();

        mvaddch(y, x, thing);
        refresh();

        int c = getch();
        switch (c) {
            case KEY_DOWN:
                y++;
                break;
            case KEY_UP:
                y--;
                break;
            case KEY_LEFT:
                x--;
                break;
            case KEY_RIGHT:
                x++;
                break;
            case 'p':
                pen_down = !pen_down;
                break;
            case 'q':
            case 'Q':
                done = true;
                break;
            default:
                if (c < 128 && c > 0) {
                    thing = (char)c;
                }
                break;
        }
    }

    endwin();
    return 0;
}
