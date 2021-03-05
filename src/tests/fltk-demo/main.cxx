#include "gui.h"

int main (int argc, char *argv[])
{
	Fl_Double_Window *window;
	window = make_window();
	window->show();
	return Fl::run();
}
