#include "../h/main.h"
#define PORT NXT_PORT_S1

/// DO NOT DELETE THIS METHOD
/// It is called every 1ms and e.g. can be used for implementing a
/// real time counter / clock.
void user_1ms_isr_type2(void) {
}

void ecrobot_device_initialize() {
	 ecrobot_set_light_sensor_active(PORT);
}

void ecrobot_device_terminate(void) {
	ecrobot_set_light_sensor_inactive(PORT);
}

void hello_world() {
	ecrobot_status_monitor("Hello, World!");

}

TASK(OSEK_Main_Task) {
	/*while (1) {
		hello_world();
		/* 500msec wait */
		/*systick_wait_ms(500);
	}*/

	while(1) {
		nxt_motor_set_speed(NXT_PORT_B, -65, 0);
		nxt_motor_set_speed(NXT_PORT_C, -65, 0);
	      display_clear(0);
	      display_goto_xy(0,0);
	      display_int(ecrobot_get_light_sensor(PORT),4);
	      display_update();
	      systick_wait_ms(500);
	      if(ecrobot_get_light_sensor(NXT_PORT_S1) <= 600) {
	    	  nxt_motor_set_speed(NXT_PORT_B, 0, 1);
	    	  nxt_motor_set_speed(NXT_PORT_C, 0, 1);
	    	  break;
	      }
	      if(ecrobot_get_touch_sensor(NXT_PORT_S2) == 1 || ecrobot_get_touch_sensor(NXT_PORT_S3) == 1) {
	    	  nxt_motor_set_speed(NXT_PORT_B, 0, 1);
	    	  nxt_motor_set_speed(NXT_PORT_C, 0, 1);

	      }
	}

	while(1) {
		systick_wait_ms(50);
	}

}
/*TASK(task1) {
	while(1) {
		if (ecrobot_is_ENTER_button_pressed()){
			nxt_motor_set_speed(PORT, 50, 0);
		}
	}
}*/

TASK(task2) {
  while(1) {
    if (ecrobot_is_ENTER_button_pressed()){
      display_clear(0);
      display_goto_xy(0,0);
      display_int(ecrobot_get_light_sensor(PORT),4);
      display_update();
    }
  }
}

