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
int rotate();
int get_intersection(int direction);
int rotate_explore(int translated_direction[4]);


#endif /* HARDWARE_H_ */
