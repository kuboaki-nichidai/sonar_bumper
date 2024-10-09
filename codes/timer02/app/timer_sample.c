#include "timer_sample.h"

typedef enum { INIT,
  WAIT01, RUNNING01,
  WAIT02, RUNNING02, STOPPED
} timer_sample_state;
timer_sample_state ts_state = INIT;

void timer_sample_config(void) {
  init_f("timer_sample");
  line_tracer_config();
}

bool ts_is_entry = true;

void timer_sample_run(void) {
  num_f(ts_state, 2); // <1>
  switch(ts_state) {
  case INIT:
    if( ts_is_entry ) {
      timer_sample_config();
      ts_is_entry = false;
    }
    if( true ) {
      ts_state = WAIT01;
      ts_is_entry = true;
    }
    break;
  case WAIT01:
    if( ts_is_entry ) {
      timer_start(5000 * 1000);
      ts_is_entry = false;
    }
    if (timer_is_timedout()) {
      ts_state = RUNNING01;
      ts_is_entry = true;
    }
    if( ts_is_entry ) {
      timer_stop();
    }
    break;
  case RUNNING01:
    if( ts_is_entry ) {
      timer_start(10000 * 1000);
      ts_is_entry = false;
    }
    line_tracer_run();
    if (timer_is_timedout()) {
      ts_state = WAIT02;
      ts_is_entry = true;
    }
    if( ts_is_entry ) {
      line_tracer_stop();
      timer_stop();
    }
    break;
  case WAIT02:
    if( ts_is_entry ) {
      timer_start(5000 * 1000);
      ts_is_entry = false;
    }
    if (timer_is_timedout()) {
      ts_state = RUNNING02;
      ts_is_entry = true;
    }
    if( ts_is_entry ) {
      timer_stop();
    }
    break;
  case RUNNING02:
    if( ts_is_entry ) {
      timer_start(10000 * 1000);
      ts_is_entry = false;
    }
    line_tracer_run();
    if (timer_is_timedout()) {
      ts_state = STOPPED;
      ts_is_entry = true;
    }
    if( ts_is_entry ) {
      line_tracer_stop();
      timer_stop();
    }
    break;
  case STOPPED:
    break;
  }
}
