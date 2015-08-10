#ifndef __TASK_H
#define __TASK_H

#define TASK_TEST	    0
#define TASK_TRACKER    1
#define TASK_SLEEP      2
#define TASK_STOP       3
#define TASK_USB        4
#define TASK_DEEPSLEEP	5

void task_deepSleep (void *);
void task_tracker (void *);
void task_sleep (void *);
void task_stop (void *);
void task_test (void *);
void task_usb (void *);


#endif