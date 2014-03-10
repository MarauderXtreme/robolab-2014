/*
 * hardware.h
 *
 *  Created on: 07.03.2014
 *      Author: marauder
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdlib.h>
#include "kernel.h"
/* Will be generated: */
#include "kernel_id.h"
#include "ecrobot_interface.h"

int actposx = 0;
int actposy = 0;

void display_message();
void set_velocity(int vb,int vc);
void start_robot();
void stop_robot();
void beep();
int is_black();
int get_degree_b(int degree);
int get_degree_c(int degree);
int get_token();
void set_count_zero();
int find_way_back();
int set_position_back(int degree);
void goto_intersection();
int get_intersection(int direction);
int rotate_explore(int translated_direction[4]);
void move(int posx, int posy, int direction);
void turn_left();
void turn_right();
void turn_back();
void turn_straight();
void rotate(int x, int y, int direction);


#endif /* HARDWARE_H_ */
