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

/**
 * Get the light sensor data
 */
void display_message() {
	display_clear(0);
	display_goto_xy(0,0);
	display_int(ecrobot_get_light_sensor(S1),4);
	/*display_goto_xy(0,1);
	display_string('EXTERMINATE');*/
	display_update();
	systick_wait_ms(500);
}

/**
 * Should be pretty self-explaining
 */
void start_robo() {
	nxt_motor_set_speed(B, maxpower, 0);
	nxt_motor_set_speed(C, maxpower, 0);
}
/**
 * See start_robo()
 */
void stop_robo() {
	nxt_motor_set_speed(B, 0, 1);
	nxt_motor_set_speed(C, 0, 1);
}

/**
 * BEEEEEP
 */
void beep() {
	ecrobot_sound_tone(300,500,40);
}

long line_degree = 660;
int rotate_degree = 60;

/**
 * give deegres of turn from b back
 */
int count() {
	display_clear(0);
	display_goto_xy(0,0);
	display_int(nxt_motor_get_count(B),4);
	display_update();
	systick_wait_ms(50);
	return 0;
}

void line_follower() {
	/**
	 * firstturn right
	 */
	nxt_motor_set_speed(B, 40, 0);
	nxt_motor_set_speed(C,-60, 0);

	nxt_motor_set_count(B, 0);
	nxt_motor_set_count(C, 0);
	
	/**
	 * turnback left
	 */
	if(nxt_motor_get_count(B) <= -50) {
		nxt_motor_set_speed(B, -60, 0);
		nxt_motor_set_speed(C, 40, 0);
		nxt_motor_set_count(B, 0);
		nxt_motor_set_count(C, 0);
		/**
		 * turnback right
		 */
		if(nxt_motor_get_count(C) >= 30) {
			nxt_motor_set_speed(B, 40, 0);
			nxt_motor_set_speed(C, -60, 0);
			nxt_motor_set_count(B, 0);
			nxt_motor_set_count(C, 0);
		}
	}
	/**
	 * turnback left
	 */
	if(nxt_motor_get_count(B) >= 30) {
		nxt_motor_set_speed(B, 40, 0);
		nxt_motor_set_speed(C, -60, 0);
		nxt_motor_set_count(B, 0);
		nxt_motor_set_count(C, 0);

		/**
		 * turnback right
		 */
		if(nxt_motor_get_count(C) >= -50) {
			nxt_motor_set_speed(B, -60, 0);
			nxt_motor_set_speed(C, 40, 0);
			nxt_motor_set_count(B, 0);
			nxt_motor_set_count(C, 0);
		}
	}

}


TASK(OSEK_Main_Task) {
	while(1) {
		display_message();
		while(ecrobot_get_light_sensor(S1) >= 540) {
			start_robo();
		}
		if(ecrobot_get_light_sensor(S1) <= 540) {
			line_follower();
		}

	}

	/*if(ecrobot_get_light_sensor(S1) <= 600) {
		stop_robo();
		beep();
	}
	else {
		start_robo();
	}*/

	if(ecrobot_get_touch_sensor(S2) == 1 || ecrobot_get_touch_sensor(S3) == 1) {
		stop_robo();
		beep();
	}
	else {
		/**
		 * Prevent that Robot will start immediately if token is removed
		 */
		systick_wait_ms(100);
	}

	/**
	 * Prevent state unclear if breaking main while(1)
	 */
	while(1) {
		systick_wait_ms(1);

	}
}
