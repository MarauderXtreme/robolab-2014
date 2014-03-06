#include "../h/main.h"

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
 * DO NOT DELETE THIS METHOD
 * It is called every 1ms and e.g. can be used for implementing a
 * real time counter / clock.
 */
void user_1ms_isr_type2(void) {
}

void ecrobot_device_initialize() {
	 ecrobot_set_light_sensor_active(S1);
}

void ecrobot_device_terminate(void) {
	ecrobot_set_light_sensor_inactive(S1);
}

void display_message() {
	display_clear(0);
	display_goto_xy(0,0);
	display_int(ecrobot_get_light_sensor(S1),4);
	display_goto_xy(0,1);
	display_string('EXTERMINATE');
	display_update();
	systick_wait_ms(500);
}

void start_robo() {
	nxt_motor_set_speed(B, -65, 0);
	nxt_motor_set_speed(C, -65, 0);
}

void stop_robo() {
	nxt_motor_set_speed(B, 0, 1);
	nxt_motor_set_speed(C, 0, 1);
}

void beep() {
	ecrobot_sound_tone(300,500,40);
}

void turn_left() {
	nxt_motor_set_count(B,90);
	nxt_motor_set_count(C,-90);
}

void turn_right() {
	nxt_motor_set_count(B,-90);
	nxt_motor_set_count(C,90);
}

int find_way_back() {
	while(1) {
	}
	return 0;
}
long line_degree = 660;
int rotate_degree = 60;


void find_way_back_right() {
	if(ecrobot_get_light_sensor(S1) <= 600) {
		while(ecrobot_get_light_sensor(S1) <= 600) {
			nxt_motor_set_count(B,-1);
			if (nxt_motor_get_count(B) >= 100) {
				break;
			}
		}
		while(ecrobot_get_light_sensor(S1) <= 600) {
			nxt_motor_set_count(B,1);
			if(ecrobot_get_light_sensor(S1) >= 580) {
				break;
			}
		}
	}

}

TASK(OSEK_Main_Task) {
	while(1) {
		display_message();

		if(ecrobot_get_light_sensor(S1) <= 600) {
			stop_robo();
		}
		else {
			start_robo();
		}

		if(ecrobot_get_touch_sensor(S2) == 1 || ecrobot_get_touch_sensor(S3) == 1) {
			stop_robo();
			beep();
		}
		else {
			/**
			 * Prevent that Robot will start immediately if token is removed
			 */
			systick_wait_ms(100);
			start_robo();
		}
	}
	/**
	 * Prevent state unclear if breaking while
	 */
	while(1) {
		systick_wait_ms(50);
	}
}
