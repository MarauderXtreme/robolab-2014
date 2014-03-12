#include "../h/main.h"
#include "../h/RobolabSimClient.h"
#include "../h/hardware.h"

/**
 * Define S Ports
 */
#define S1 NXT_PORT_S1
#define S2 NXT_PORT_S2
#define S3 NXT_PORT_S3
#define S4 NXT_PORT_S4

/**
 * Define alphabetical Ports
 */
#define A NXT_PORT_A
#define B NXT_PORT_B
#define C NXT_PORT_C

/**
 * define speed_level
 */
#define maxpower -80
#define mediumpower -70
#define lowpower -60

/**
 * DO NOT DELETE THIS METHOD
 * It is called every 1ms and e.g. can be used for implementing a
 * real time counter / clock.
 */
void user_1ms_isr_type2(void) {
}
/**
 * Initialize Device
 */
void ecrobot_device_initialize() {
	 ecrobot_set_light_sensor_active(S1);
}

/**
 * Terminate Device
 */
void ecrobot_device_terminate(void) {
	ecrobot_set_light_sensor_inactive(S1);
}

TASK(OSEK_Main_Task) {
	/*
	while(1) {
		while(is_black() == 1 && get_token() == 0 && got_intersection == 0) {
			start_robot();
		}
		if(get_token() == 1) {
			systick_wait_ms(10000);
		}
		if(is_black() == 0 && get_token() == 0  && got_intersection == 0) {
			if(find_way_back() == 0) {
				goto_intersection();
				got_intersection = 1;
			}
		}
		if(got_intersection == 1) {
			get_intersection(direction); //int intersection_type =
			stop_robot();
			move(0,-1,direction);
			got_intersection = 0;
		}
	}
	*/
	move(7, 5, dir);
	move(8, 5, dir);
	move(8, 4, dir);
	move(7, 4, dir);

	//start_finding(START_X, START_Y);

	/**
	 * DO NOT DELETE THIS METHOD
	 * Prevent state unclear if breaking main while(1)
	 */
	display_clear(0);
	display_goto_xy(0,0);
	display_char('EXTERMINATE');
	display_update();
	while(1) {
		systick_wait_ms(1);
	}
}
