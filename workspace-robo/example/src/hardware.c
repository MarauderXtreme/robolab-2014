/*
 * hardware.c
 *
 *  Created on: 07.03.2014
 *      Author: marauder
 */

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
 * Helper Function for moving the Roboter
 */
void set_velocity(int vb,int vc) {
	nxt_motor_set_speed(B, vb, 0);
	nxt_motor_set_speed(C, vc, 0);
}

/**
 * Should be pretty self-explaining
 */
void start_robot() {
	set_velocity(mediumpower,mediumpower);
}
/**
 * See start_robot()
 */
void stop_robot() {
	nxt_motor_set_speed(B, 0, 1);
	nxt_motor_set_speed(C, 0, 1);
}

/**
 * BEEEEEP
 */
void beep() {
	ecrobot_sound_tone(300,500,40);
}
/**
 * 1/TRUE if black
 * 0/FALSE if not black
 */
int is_black() {
	if(ecrobot_get_light_sensor(S1) >= 540) return 1;
	return 0;
}
/**
 * Helper Function for rotation
 * 1/TRUE when degree count of B is greater than degree
 */
int get_degree_b(int degree) {
	if(nxt_motor_get_count(B) >= degree) return 1;
	return 0;
}
/**
 * Helper Function for rotation
 * 1/TRUE when degree count of C is greater than degree
 */
int get_degree_c(int degree) {
	if(nxt_motor_get_count(C) >= degree) return 1;
	return 0;
}
/**
 * Initialize the servo degree counter
 */
void set_count_zero() {
	nxt_motor_set_count(B, 0);
	nxt_motor_set_count(C, 0);
}
/**
 * Detect if robot only moved right or left from line
 */
int find_way_back() {
	/**
	 * @TODO Find out if Servo C is stronger than B and switch the initial side
	 */
	set_count_zero();
	/**
	 * turn left
	 */
	while(get_degree_b(30) != 1) {
		set_velocity(60,-60);
		if(is_black() == 1) {
			stop_robot();
			return 1;
		}
	}
	stop_robot();
	/**
	 * turn right
	 */
	while(get_degree_c(30) != 1) {
		set_velocity(-60,60);
		if(is_black() == 1) {
			stop_robot();
			return 1;
		}
	}
	stop_robot();
	return 0;
}
/**
 * Helper Function for setting back position
 * 1/TRUE when degree count of C is equal to degree
 */
int set_position_back(int degree) {
	if(nxt_motor_get_count(C) == degree) return 1;
	return 0;
}
/**
 * After detecting a intersection
 * robot rotates back and
 * moves on to intersection
 */
void goto_intersection() {
	while(set_position_back(-20) != 1) {
		set_velocity(60,-60);
	}
	stop_robot();
	set_count_zero();
	while(nxt_motor_get_count(B) >= -220 && nxt_motor_get_count(C) >= -220) {
		set_velocity(mediumpower,mediumpower);
	}
	stop_robot();
}

/**
 * translates direction for detection
 * gives back the type of the actual intersection
 */
int get_intersection(direction) {
	int intersection;
	set_count_zero();
	if(direction == 0x20) {
		int translated_direction[4]={0x10,0x40,0x20,0x80};
		rotate_explore(translated_direction[4]);
	}
	if(direction == 0x80) {
		int translated_direction[4]={0x40,0x20,0x80,0x10};
		rotate_explore(translated_direction[4]);
	}
	if(direction == 0x10) {
		int translated_direction[4]={0x20,0x80,0x10,0x40};
		rotate_explore(translated_direction[4]);
	}
	if(direction == 0x40) {
		int translated_direction[4]={0x80,0x10,0x40,0x20};
		rotate_explore(translated_direction[4]);
	}
	return intersection;
}
/**
 * rotate robot and
 * explore intersection
 * gives back intersection
 */
int rotate_explore(int translated_direction[4]) {
	int intersection = 0x00;
	while(get_degree_b(360) != 1) {
		set_velocity(60,-60);
		if(is_black() == 1 && nxt_motor_get_count(B) <= 60) {
			intersection = intersection + translated_direction[0];
		}
		if(is_black() == 1 && nxt_motor_get_count(B) <= 150) {
			intersection = intersection + translated_direction[1];
		}
		if(is_black() == 1 && nxt_motor_get_count(B) <= 240) {
			intersection = intersection + translated_direction[2];
		}
		if(is_black() == 1 && nxt_motor_get_count(B) <= 330) {
			intersection = intersection + translated_direction[3];
		}
	}
	return intersection;
}
