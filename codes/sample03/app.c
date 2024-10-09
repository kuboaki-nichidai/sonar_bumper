#include "app.h"
#include "util.h"

const int bumper_sensor = EV3_PORT_1; // <1>
const int linemon_sensor = EV3_PORT_3;
const int left_motor = EV3_PORT_A;
const int right_motor = EV3_PORT_C;

int bumper_is_pushed(void) { // <2>
  return ev3_touch_sensor_is_pressed(bumper_sensor);
}

#define LM_THRESHOLD 20
int lm_threshold = LM_THRESHOLD;

int linemon_is_online(void) {
  return ev3_color_sensor_get_reflect(linemon_sensor) < lm_threshold;
}

#define DR_POWER 20
int dr_power = DR_POWER;

void driver_turn_left(void) {
  ev3_motor_set_power(EV3_PORT_A, 0);
  ev3_motor_set_power(EV3_PORT_C, dr_power);
}

void driver_turn_right(void) {
  ev3_motor_set_power(EV3_PORT_A, dr_power);
  ev3_motor_set_power(EV3_PORT_C, 0);
}

void driver_stop(void) { // <1>
  ev3_motor_stop(left_motor, false);
  ev3_motor_stop(right_motor, false);
}

void tracer_run(void) {
  if( linemon_is_online() ) {
    driver_turn_left();
  } else {
    driver_turn_right();
  }
}

void tracer_stop(void) { // <1>
  driver_stop();
}

enum { TRANSPORTING, ARRIVED }; // <1>
int porter_state = TRANSPORTING; // <2>

void porter_transport(void) {
  switch(porter_state) { // <3>
  case TRANSPORTING: // <4>
    tracer_run();
    if( bumper_is_pushed() ) { // <5>
      porter_state = ARRIVED;  // <5>
    }
    break;
  case ARRIVED: // <6>
    tracer_stop();
    break;
  }
}

void main_task(intptr_t unused) {
  static int is_initialized = false;
  if(! is_initialized ) {
    is_initialized = true;
    init_f("sample03");
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);
    ev3_sensor_config(linemon_sensor, COLOR_SENSOR);
    ev3_sensor_config(bumper_sensor, TOUCH_SENSOR); // <1>
  }

  porter_transport();
  ext_tsk();
}
