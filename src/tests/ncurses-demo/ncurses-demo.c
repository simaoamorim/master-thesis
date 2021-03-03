#include <locale.h>
#include <curses.h>

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "ISO-8859-1");
	initscr();
	cbreak();
	noecho();
	raw();

	keypad(stdscr, TRUE);
	printw("Hello world!\n");
	printw("I'm an ncurses application");
	mvprintw(LINES-1, 0, "Press q to quit");

	refresh();
	int ch;
	while ('q' != (ch = getch())) {
		WINDOW *tmp_window = newwin(5, 70, (LINES-5)/2, (COLS-70)/2);
		box(tmp_window, 0, 0);
		int l, c;
		mvwprintw(tmp_window, 1, 1, "Did not press q");
		getyx(tmp_window, l, c);
		mvwprintw(tmp_window, l+1, 1, "Press [return] to close this window");
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
