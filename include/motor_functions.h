#ifndef MOTOR_FUNCTIONS_H
#define MOTOR_FUNCTIONS_H

#include <Arduino.h>

extern GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;


void stepper_act(int clockwise, int duty);
void intialise_pump(int clockwise, int duty);
void intermittent_sampling(int on_time, int off_time, int duty);
void updateDisplay(String delay, String sampling, String time_remaining, bool waiting);

#endif // MY_FUNCTIONS_H