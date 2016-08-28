#include "stdafx.h"
#include "helper.h"

int row = 25;
int column = 80;
int step_time = 1000000;  // 1000000 for 1s
int buffer_flush_interval = 1000;
void(*resize_event_handler)();

bool _screen_buffer_flush_enabled = false;
bool _screen_buffer_draw_lock = false;
pthread_t screen_buffer_flush_thread;

void resize_event_handler_wrapper(int sig)
{
	signal(SIGWINCH, SIG_IGN); 							/* disable interrupt */
	//re_init_screen();									/* re-initialize window */
	resize_event_handler();
	signal(SIGWINCH, &resize_event_handler_wrapper); 	/* enable interrupt */
}

void set_resize_event_handler(void(*func)(int))
{
	resize_event_handler = func;
	resize_event_handler();
	signal(SIGWINCH, &resize_event_handler_wrapper);
}

void unset_resize_event_handler()
{
	signal(SIGWINCH, SIG_IGN);
}

void update_screen_metrics(){
	column = COLS;
	row = LINES;
	printw("%d, %d ", column, row);
}

void *flush_screen_buffer(void *param)
{
	while(_screen_buffer_flush_enabled && !_screen_buffer_draw_lock)
	{
		refresh();
		usleep(buffer_flush_interval);
	}
	return NULL;
}

void wait_buffer_lock()
{
	while(_screen_buffer_draw_lock) usleep(buffer_flush_interval);
}

void lock_buffer()
{
	_screen_buffer_draw_lock = true;
}

void unlock_buffer()
{
	_screen_buffer_draw_lock = false;
}

void init_screen()
{
	initscr();
	cbreak();
	nodelay(stdscr, TRUE);
	// halfdelay(step_time); /* set a timeout of key reading */
	noecho();
	keypad(stdscr, TRUE);
	clear();
	set_resize_event_handler(update_screen_metrics);
	_screen_buffer_flush_enabled = true;
	if(pthread_create(&screen_buffer_flush_thread, NULL, flush_screen_buffer, NULL)) {
		fprintf(stderr, "Error creating screen buffer flush thread\n");
		return;
	}
}

void re_init_screen()
{
	endwin();
	init_screen();
}

void end_screen()
{
	_screen_buffer_flush_enabled = false;
	if(pthread_join(screen_buffer_flush_thread, NULL)) {
		fprintf(stderr, "Error joining screen buffer flush thread\n");
		return;
	}
	endwin();
}

