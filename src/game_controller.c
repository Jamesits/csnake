#include "stdafx.h"
#include "game_controller.h"

void game_loop()
{
	printw("Hello World !!!");	/* Print Hello World		  */
	refresh();			/* Print it on to the real screen */
	getch();			/* Wait for user input */
}