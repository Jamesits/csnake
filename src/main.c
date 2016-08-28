#include "stdafx.h"
#include "game_controller.h"

int main(void)
{
	initscr();			/* Start curses mode 		  */
	game_loop();
	endwin();			/* End curses mode		  */
	return 0;
}