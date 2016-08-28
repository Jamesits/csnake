#include "stdafx.h"
#include "game_controller.h"
#include "terminal_seq.h"
#include "helper.h"

bool running = true;

void interrupt()
{
	running = false;
}

void on_key_press(int ch)
{
	wait_buffer_lock();
	printf("%d", ch);
}

void *game_timer(void *param)
{
	int i = 0;
	while(running)
	{
		wait_buffer_lock();
		lock_buffer();
		printf("Thread 1: %d\n", i++);
		unlock_buffer();
		usleep(step_time);
	}
	return NULL;
}

void game_loop()
{
	/* listen interrupt on ^C */
	signal(SIGINT, interrupt);

	/* start a timer on thread 1 */
	pthread_t game_timer_thread;
	if(pthread_create(&game_timer_thread, NULL, game_timer, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return;
	}

	/* draw a border */
	box(stdscr, L'😊', L'█');

	/* read keyboard action on thread 0 */
	while(running)
	{
		int ch = getch();
		if (ch != ERR) on_key_press(ch);
	}

	/* catch the timer */
	if(pthread_join(game_timer_thread, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return;
	}
}