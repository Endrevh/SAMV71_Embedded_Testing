#include <atmel_start.h>


static void TIMER_0_task1_callback(const struct timer_task *const timer_task);
static void TIMER_0_task2_callback(const struct timer_task *const timer_task);

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	struct timer_task TIMER_0_task1, TIMER_0_task2;

	TIMER_0_task1.interval = 999;
	TIMER_0_task1.cb       = TIMER_0_task1_callback;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_0_task2.interval = 2000;
	TIMER_0_task2.cb       = TIMER_0_task2_callback;
	TIMER_0_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_add_task(&TIMER_0, &TIMER_0_task2);
	timer_start(&TIMER_0);
	
	
	/* Replace with your application code */
	while (1) {
		
	}
}

static void TIMER_0_task1_callback(const struct timer_task *const timer_task)
{
	printf("Timer 0 time is: %d \n", TIMER_0.time);
}

static void TIMER_0_task2_callback(const struct timer_task *const timer_task)
{
}