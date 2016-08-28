#include "stdafx.h"
#include "game_controller.h"
#include "helper.h"

int main(void)
{
	setlocale(LC_ALL, "");
	init_screen();
	game_loop();
	end_screen();
	return 0;
}