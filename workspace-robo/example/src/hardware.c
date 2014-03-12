/*
 * hardware.c
 *
 *  Created on: 07.03.2014
 *      Author: marauder
 */

#include "../h/main.h"
#include "../h/hardware.h"
#include "../h/RobolabSimClient.h"

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
#define maxpowneg -80
#define medpowneg -70
#define lowpowneg -65

#define maxpowpos 80
#define medpowpos 70
#define lowpowpos 65

#define powpos medpowpos
#define powneg medpowneg

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
	set_velocity(powneg,powneg);
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
	ecrobot_sound_tone(300,500,50);
}
void beep2() {
	ecrobot_sound_tone(600,500,50);
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
 * Detect the token over touch
 * 1/TRUE when Touch-Sensor is 1
 */
int get_token() {
	if(ecrobot_get_touch_sensor(S2) == 1 || ecrobot_get_touch_sensor(S3) == 1) {
		stop_robot();
		beep();
		return 1;
	}
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
	set_count_zero();
	/**
	 * turn right
	 */
	while(get_degree_c(50) != 1) {
		set_velocity(lowpowneg,lowpowpos);
		if(is_black() == 1) {
			stop_robot();
			return 1;
		}
	}
	stop_robot();
	/**
	 * turn left
	 */
	while(get_degree_b(50) != 1) {
		set_velocity(lowpowpos,lowpowneg);
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
	if(nxt_motor_get_count(B) == degree) return 1;
	return 0;
}
/**
 * After detecting a intersection
 * robot rotates back and
 * moves on to intersection
 */
void goto_intersection() {
	while(set_position_back(-20) != 1) {
		set_velocity(lowpowneg,lowpowpos);
	}
	stop_robot();
	set_count_zero();
	while(nxt_motor_get_count(B) >= -200 && nxt_motor_get_count(C) >= -200) {
		set_velocity(powneg,powneg);
	}
	stop_robot();
}

/**
 * translates direction for detection
 * gives back the type of the actual intersection
 */
int get_intersection(int direction) {
	int intersection = ROBOT_FAIL;
	if(direction == SOUTH) {
		int translated_direction[4]={WEST,SOUTH,EAST,NORTH};
		intersection = rotate_explore(translated_direction);
	}
	if(direction == EAST) {
		int translated_direction[4]={SOUTH,EAST,NORTH,WEST};
		intersection = rotate_explore(translated_direction);
	}
	if(direction == NORTH) {
		int translated_direction[4]={EAST,NORTH,WEST,SOUTH};
		intersection = rotate_explore(translated_direction);
	}
	if(direction == WEST) {
		int translated_direction[4]={NORTH,WEST,SOUTH,EAST};
		intersection = rotate_explore(translated_direction);
	}
	return intersection;
}

/**
 * rotate robot and
 * explore intersection
 * gives back intersection
 */
int rotate_explore(int translated_direction[4]) {
	int intersection = 0;
	int first_flag = 0;
	int second_flag = 0;
	int third_flag = 0;
	int fourth_flag = 0;
	set_count_zero();
	while(get_degree_b(850) != 1) {
		set_velocity(medpowpos,medpowneg);
		if(is_black() == 1 && nxt_motor_get_count(B) >= 100 && nxt_motor_get_count(B) <= 270) {
			beep();
			first_flag = 1;
		}
		if(is_black() == 1 && nxt_motor_get_count(B) >= 370 && nxt_motor_get_count(B) <= 440) {
			beep();
			second_flag = 1;
		}
		if(is_black() == 1 && nxt_motor_get_count(B) >= 520 && nxt_motor_get_count(B) <= 660) {
			beep();
			third_flag = 1;
		}
		if(is_black() == 1 && nxt_motor_get_count(B) >= 770 && nxt_motor_get_count(B) <= 840) {
			beep();
			fourth_flag = 1;
		}
	}
	stop_robot();
	if(first_flag == 1) {
		intersection = intersection + translated_direction[0];
	}
	if(second_flag == 1) {
		intersection = intersection + translated_direction[1];
	}
	if(third_flag == 1) {
		intersection = intersection + translated_direction[2];
	}
	if(fourth_flag == 1) {
		intersection = intersection + translated_direction[3];
	}
	return intersection;
}

int move(int posx, int posy, int direction) {
	int ret = ROBOT_FAIL;
	drive(posx, posy, direction);
	while(1) {
		while(is_black() == 1 && get_token() == 0)
		{
			start_robot();
		}
		if(is_black() == 0 && get_token() == 0) {
			if(find_way_back() == 0) {
				goto_intersection();
				if (ret != ROBOT_TOKENFOUND) {
					ret = ROBOT_SUCCESS;
				}
				break;
			}
		}
		if(get_token() == 1) {
			systick_wait_ms(10000);
			ret = ROBOT_TOKENFOUND;
		}
	}
	beep2();
	return ret;
}

void drive(int posx, int posy, int direction) {
	int x = posx - actposx;
	int y = posy - actposy;
	actposx = posx;
	actposy = posy;
	g_dir = rotate(x,y,direction);
}

int rotate(int x, int y, int direction) {
	set_count_zero();
	switch (direction) {
		case NORTH:
			if(x == -1 && y == 0) {
				turn_right();
				direction = EAST;
			}
			if(x == 1 && y == 0) {
				turn_left();
				direction = WEST;
			}
			if(x == 0 && y == 1) {
				turn_back();
				direction = SOUTH;
			}
			if(x == 0 && y == -1) {
				turn_straight();
				direction = NORTH;
			}
			break;
		case SOUTH:
			if(x == 1 && y == 0) {
				turn_right();
				direction = WEST;
			}
			if(x == -1 && y == 0) {
				turn_left();
				direction = EAST;
			}
			if(x == 0 && y == -1) {
				turn_back();
				direction = NORTH;
			}
			if(x == 0 && y == 1) {
				turn_straight();
				direction = SOUTH;
			}
			break;
		case WEST:
			if(x == 0 && y == -1) {
				turn_right();
				direction = NORTH;
			}
			if(x == 0 && y == 1) {
				turn_left();
				direction = SOUTH;
			}
			if(x == -1 && y == 0) {
				turn_back();
				direction = EAST;
			}
			if(x == 1 && y == 0) {
				turn_straight();
				direction = WEST;
			}
			break;
		case EAST:
			if(x == 0 && y == 1) {
				turn_right();
				direction = SOUTH;
			}
			if(x == 0 && y == -1) {
				turn_left();
				direction = NORTH;
			}
			if(x == 1 && y == 0) {
				turn_back();
				direction = WEST;
			}
			if(x == -1 && y == 0) {
				turn_straight();
				direction = EAST;
			}
			break;
		default:
			beep2();
			break;
	}
	return direction;
}

void turn_left() {
	while(get_degree_b(230) != 1) {
		set_velocity(medpowpos,medpowneg);
		if(is_black() == 1 && nxt_motor_get_count(B) >= 50 && nxt_motor_get_count(B) <= 230) {
			stop_robot();
			return;
		}
	}
}
void turn_right() {
	while(get_degree_c(400) != 1) {
		set_velocity(medpowneg,medpowpos);
		if(is_black() == 1 && nxt_motor_get_count(C) >= 230 && nxt_motor_get_count(C) <= 400) {
			stop_robot();
			return;
		}
	}
}
void turn_back() {
	while(get_degree_b(400) != 1) {
		set_velocity(medpowpos,medpowneg);
		if(is_black() == 1 && nxt_motor_get_count(B) >= 230 && nxt_motor_get_count(B) <= 400) {
			stop_robot();
			return;
		}
	}
}
void turn_straight() {
	while(get_degree_c(200) != 1) {
		set_velocity(medpowneg,medpowpos);
		if(is_black() == 1 && nxt_motor_get_count(C) >= 50 && nxt_motor_get_count(C) <= 200) {
			stop_robot();
			return;
		}
	}
}
