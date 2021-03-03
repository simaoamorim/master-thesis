#include <locale.h>
#include <curses.h>

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "ISO-8859-1");
	initscr();
	cbreak();
	noecho();
	raw();
	curs_set(0);

	keypad(stdscr, TRUE);
	printw("Hello world!\n");
	printw("I'm an ncurses application");

	mvprintw(LINES-1, 0, "Press q to quit");

	refresh();
	int ch;
	while ('q' != (ch = getch())) {
		WINDOW *tmp_window = newwin(4, 50, (LINES-4)/2, (COLS-50)/2);
		box(tmp_window, 0, 0);
		mvwprintw(tmp_window, 1, 1, "Did not press q");
		mvwprintw(tmp_window, 2, 1, "Press [return] to close this window");
		wrefresh(tmp_window);
		getch();
		wclear(tmp_window);
		wrefresh(tmp_window);
		delwin(tmp_window);
		refresh();
	}

	endwin();
	return 0;
}
