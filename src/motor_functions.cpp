#include <Arduino.h>
#include "esp_sleep.h"


// Set Pump Pins 
#define pump_pin 12
#define dir_pin 13
#define en_pin 14
#define led 10



void stepper_act(int clockwise, int duty);
void intialise_pump(int clockwise, int duty);
void intermittent_sampling(int on_time, int off_time, int duty);


void stepper_act(int clockwise, int duty) { //todo: direction
  //enable the stepper motor pin to hold the torque
  //int timer = 0;
  digitalWrite(en_pin, HIGH);
  //from Pico_1.4_Peristaltic_Pump_Driver.pdf
  //Open (or +5.0 V) = direction anti-clockwise / GND = direction clockwise
  if (clockwise){
    digitalWrite(dir_pin, LOW);
  } else{
    digitalWrite(dir_pin, HIGH);
  }
  if (duty == 0) {
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    //disable the drive pin
    analogWrite(pump_pin, 0);
    //disable the enable pin
    digitalWrite(en_pin, LOW);
  } else {
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      analogWrite(pump_pin, duty * 1023 / 100);    
    }
}


void intialise_pump(int clockwise, int duty) { //todo: direction
  //enable the stepper motor pin to hold the torque
  //int timer = 0;
  digitalWrite(en_pin, HIGH);
  //from Pico_1.4_Peristaltic_Pump_Driver.pdf
  //Open (or +5.0 V) = direction anti-clockwise / GND = direction clockwise
  if (clockwise){
    digitalWrite(dir_pin, LOW);
  } else{
    digitalWrite(dir_pin, HIGH);
  }
  if (duty == 0) {
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    //disable the drive pin
    analogWrite(pump_pin, 0);
    //disable the enable pin
    digitalWrite(en_pin, LOW);
  } else {
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      analogWrite(pump_pin, duty * 1023 / 100);    
    }
  delay(120000);
  stepper_act(1, 0);
}

// Implementation of intermittent sampling
// start_delay: Hours   on_time: seconds    off_time: seconds   duty: 1-100
void intermittent_sampling(int on_time, int off_time, int duty){
  int counter = 0;
  int flag = 1;
  esp_sleep_enable_timer_wakeup(off_time * 1000000ULL); // off_time in seconds
  while (true){
    if (counter == 24*60*60/(on_time + off_time) + 1) { //24 Hour runtime
      // Stop the loop after 2880 iterations
      while (true) {
        // Infinite loop to halt execution
      }
    }  
    else if (flag == 1) {
      stepper_act(1, duty); // Turn pump on
      delay(on_time*1000); // 3s
      flag = 2; //
    }
    
  
    else if(flag == 2){
      stepper_act(1, 0); // Turn pump off
      counter++; // Iterate the counter
      esp_light_sleep_start(); // Light Sleep & Resume after this line
      flag = 1; // Send system back to pump on (flag = 1)
    }
  }
}