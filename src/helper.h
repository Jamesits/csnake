#pragma once
#ifndef __HELPER_H__
#define __HELPER_H__

extern int row, column, step_time;
void init_screen();
void re_init_screen();
void end_screen();
void set_resize_event_handler(void(*)());
void unset_resize_event_handler();
void wait_buffer_lock();
void lock_buffer();
void unlock_buffer();

#endif