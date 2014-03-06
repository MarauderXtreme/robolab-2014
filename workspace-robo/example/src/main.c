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
	/*display_goto_xy(0,1);
	display_string('EXTERMINATE');*/
	display_update();
	systick_wait_ms(500);
}

void start_robo() {
	nxt_motor_set_speed(B, mediumpower, 0);
	nxt_motor_set_speed(C, mediumpower, 0);
}
void stop_robo() {
	nxt_motor_set_speed(B, 0, 1);
	nxt_motor_set_speed(C, 0, 1);
}

void beep() {
	ecrobot_sound_tone(300,500,40);
}

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

int is_black() {
	if(ecrobot_get_light_sensor(S1) >= 540) return 1;
	//if(ecrobot_get_light_sensor(S1) <= 540) return 0;
	return 0;
}

int get_degree_b(int degree) {
	if(nxt_motor_get_count(B) >= degree) return 1;
	//if(nxt_motor_get_count(B) < degree) return 0;
	return 0;
}
int get_degree_c(int degree) {
	if(nxt_motor_get_count(C) >= degree) return 1;
	//if(nxt_motor_get_count(C) < degree) return 0;
	return 0;
}
void set_count_zero() {
	nxt_motor_set_count(B, 0);
	nxt_motor_set_count(C, 0);
}
void find_way_back() {
	set_count_zero();
	while(get_degree_b(30) != 1) {
		nxt_motor_set_speed(B,  60, 0);
		nxt_motor_set_speed(C, -60, 0);
		if(is_black() == 1) {
			stop_robo();
			return;
		}
	}
	stop_robo();
	while(get_degree_c(30) != 1) {
		nxt_motor_set_speed(B, -60, 0);
		nxt_motor_set_speed(C,  60, 0);
		if(is_black() == 1) {
			stop_robo();
			return;
		}
	}
	stop_robo();
}

void set_position_back() {
	while(get_degree_b(0) != 1 && get_degree_c(0) != 1) {
		nxt_motor_set_speed(B,  60, 0);
		nxt_motor_set_speed(C, -60, 0);
	}
	stop_robo();
}

TASK(OSEK_Main_Task) {
	while(1) {
		if(is_black() == 1) {
			start_robo();
		}
		if(is_black() == 0) {
			find_way_back();
			set_position_back();
		}
	}
	/**
	 * Prevent state unclear if breaking while
	 */
	while(1) {
		systick_wait_ms(1);
	}
}
