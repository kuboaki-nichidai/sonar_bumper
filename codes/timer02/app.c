#include "ev3api.h"
#include "app.h"
#include "timer_sample.h"

void main_task(intptr_t unused) {
  timer_sample_run();
  ext_tsk();
}
