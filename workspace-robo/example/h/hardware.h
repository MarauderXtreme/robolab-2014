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

#include "main.h"
#include "RobolabSimClient.h"

void display_message();
void set_velocity(int vb,int vc);
void start_robot();
void stop_robot();
void beep();
void beep2();
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
int move(int posx, int posy, int direction);
void drive(int posx, int posy, int direction);
int rotate(int x, int y, int direction);
void turn_left();
void turn_right();
void turn_back();
void turn_straight();


#endif /* HARDWARE_H_ */
