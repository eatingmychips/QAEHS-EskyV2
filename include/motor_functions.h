#ifndef MOTOR_FUNCTIONS_H
#define MOTOR_FUNCTIONS_H

#include <Arduino.h>

void stepper_act(int clockwise, int duty);
void intialise_pump(int clockwise, int duty);
void intermittent_sampling(int on_time, int off_time, int duty);

#endif // MY_FUNCTIONS_H