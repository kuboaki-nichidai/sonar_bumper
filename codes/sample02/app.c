#include "app.h"
#include "util.h"

const int color_sensor = EV3_PORT_3; // <1>
const int left_motor = EV3_PORT_A;
const int right_motor = EV3_PORT_C;

#define LIGHT_THRESHOLD 20 // <2>

void main_task(intptr_t unused) {
  static int is_initialized = false;
  if(! is_initialized ) { // <3>
    is_initialized = true;
    init_f("sample02");
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);
    ev3_sensor_config(color_sensor, COLOR_SENSOR);
  }

  if( ev3_color_sensor_get_reflect(color_sensor) < LIGHT_THRESHOLD ) { // <4>
    ev3_motor_set_power(EV3_PORT_A, 0);
    ev3_motor_set_power(EV3_PORT_C, 20);
  } else {
    ev3_motor_set_power(EV3_PORT_A, 20);
    ev3_motor_set_power(EV3_PORT_C, 0);
  }

  ext_tsk();
}
