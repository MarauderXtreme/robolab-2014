#ifndef MAIN_H_
#define MAIN_H_

#include <stdlib.h>
#include "kernel.h"
/* Will be generated: */
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* add user headers here: */

#include "RobolabSimClient.h"


// Tells us where we came from
int g_dir = 0x20;

//Define actual start position
int actposx = 6;
int actposy = 7;

#endif /* MAIN_H_ */
