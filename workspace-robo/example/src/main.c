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

void set_velocity(int vb,int vc) {
	nxt_motor_set_speed(B, vb, 0);
	nxt_motor_set_speed(C, vc, 0);
}

/**
 * Should be pretty self-explaining
 */
void start_robo() {
	set_velocity(mediumpower,mediumpower);
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

int is_black() {
	if(ecrobot_get_light_sensor(S1) >= 540) return 1;
	return 0;
}

int get_degree_b(int degree) {
	if(nxt_motor_get_count(B) >= degree) return 1;
	return 0;
}
int get_degree_c(int degree) {
	if(nxt_motor_get_count(C) >= degree) return 1;
	return 0;
}
void set_count_zero() {
	nxt_motor_set_count(B, 0);
	nxt_motor_set_count(C, 0);
}
int find_way_back() {
	set_count_zero();
	/**
	 * @TODO Find out if Servo C is stronger than B and switch the initial side
	 */
	while(get_degree_b(30) != 1) {
		set_velocity(60,-60);
		if(is_black() == 1) {
			stop_robo();
			return 1;
		}
	}
	stop_robo();
	while(get_degree_c(30) != 1) {
		set_velocity(-60,60);
		if(is_black() == 1) {
			stop_robo();
			return 1;
		}
	}
	stop_robo();
	return 0;
}

int set_position_back_helper(int degree) {
	if(nxt_motor_get_count(C) == degree) return 1;
	return 0;
}

void goto_intersection() {
	while(set_position_back_helper(-20) != 1) {
		set_velocity(60,-60);
	}
	stop_robo();
	set_count_zero();
	while(nxt_motor_get_count(B) >= -220 && nxt_motor_get_count(C) >= -220) {
		set_velocity(mediumpower,mediumpower);
	}
	stop_robo();
}

TASK(OSEK_Main_Task) {
	while(1) {
		int got_intersection = 0;
		while(is_black() == 1 && got_intersection == 0) {
			start_robo();
		}
		if(is_black() == 0  && got_intersection == 0) {
			if(find_way_back() == 0) {
				goto_intersection();
				got_intersection = 1;
			}
		}

	}

	/**
	 * Prevent state unclear if breaking main while(1)
	 */
	while(1) {
		systick_wait_ms(1);
	}
}
