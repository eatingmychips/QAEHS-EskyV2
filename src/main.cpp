#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <esp_sleep.h>
#include "RTClib.h"
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <EEPROM.h>
#include <motor_functions.h>

// RTC memory variables to retain state across deep sleep
RTC_DATA_ATTR bool initial_delay_done = false;

//e-Paper Display Connections
#define EPD_CS      5     // GPIO5
#define EPD_DC      17    // GPIO17
#define EPD_RST     16    // GPIO16
#define EPD_BUSY    4     // GPIO4

#define EPD_DIN    23     //MOSI
#define EPD_CLK    18     //SCK

// Display model (GDEH0154D67)
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(
  GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));


const uint16_t RecvPin = 6;
IRrecv irrecv(RecvPin);
decode_results results;  
unsigned long IRCode = 0;

#define ONE 0xBA45FF00   // HEX code for the 1 button
#define TWO 0xB946FF00 // HEX code for the 2 button
#define THREE 0xB847FF00 // HEX code for the 3 button
#define FOUR 0xBB44FF00 
#define FIVE 0xBF40FF00
#define SIX 0xBC43FF00
#define SEVEN 0xF807FF00
#define EIGHT 0xEA15FF00
#define NINE 0xF609FF00
#define STAR 0xE916FF00
#define ZERO 0xE619FF00
#define HASH 0xF20DFF00 // HEX code for the # button 
#define OK 0xE31CFF00  // HEX code for the OK button
void updateDisplay(String delay, String sampling, String time_remaining); 

// Set Pump Pins 
#define pump_pin 12
#define dir_pin 13
#define en_pin 14


// Set constants
int led = 13;
int flag = 1;
int analog_write_freq = 146485;
int duty_cycle = 10;

void setup() {
  EEPROM.begin(512);
  Serial.begin(115200);
  display.init();
  display.setRotation(1);  // Landscape orientation
  display.setFullWindow();
  display.firstPage();
  display.fillScreen(GxEPD_WHITE);
  
  
  pinMode(pump_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT); 
  pinMode(en_pin, OUTPUT); 

  // Display initial content
  updateDisplay("N.A", "No", "N.A");
  irrecv.enableIRIn();

  if (initial_delay_done == true){
    intermittent_sampling(3, 27, duty_cycle);
  }
}

// TODO: Change Intermittent Sampling to no delay, include deep sleep delay in code,
// and add setup logic to continue intermittent sampling. 
void loop() {
  updateDisplay("N.A", "No", "N.A");
  while (IRCode == 0){ 
    if (irrecv.decode(&results)){
      Serial.println(IRCode, HEX);
      if ((IRCode == OK || IRCode == ONE || IRCode == TWO || IRCode == THREE)){ // If IR value received is valid
        continue;
      }
      else if (IRCode == HASH) {
        intialise_pump(1, duty_cycle);
        IRCode = 0;
      }
      else {
        IRCode = 0;
        Serial.println(IRCode);
      }
      irrecv.resume(); // Receive the next value
    }
  }
  if (IRCode == OK) { // Start immediately Intermittent Sampling
    updateDisplay("0 hrs", "Yes", "24 hours");
    delay(2000);
    intermittent_sampling(3, 27, duty_cycle);
  } else if (IRCode == ONE) { // Start 12 hour delay
    updateDisplay("12 Hrs", "No", "N.A");
    esp_sleep_enable_timer_wakeup(12*60*60 * 1000000ULL);
    esp_deep_sleep_start();
    intermittent_sampling(3, 27, duty_cycle);

  } else if (IRCode == TWO) { // Start immediately Continuous sampling
    updateDisplay("24 Hrs", "No", "N.A");
    intermittent_sampling(3, 27, duty_cycle);

  } else if (IRCode == THREE) { // Delay for 48 hours 
    updateDisplay("48 hrs", "No", "N.A");
    intermittent_sampling(3, 27, duty_cycle);
  }
}

void updateDisplay(String delay, String sampling, String time_remaining) {
  display.setFullWindow();
  display.firstPage();
  
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    
    // Set font
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextSize(1);
    
    // Display header
    display.setCursor(10, 20);
    display.println("QAEHS Esky V2");
    
    // Display sensor info (example)
    display.setCursor(10, 80);
    display.print("Delay Mode: ");
    display.print(delay);
    
    display.setCursor(10, 110);
    display.print("Sampling?: ");
    display.print(sampling);
    
    // Display footer
    display.setCursor(10, 140);
    display.print("Time Rem: ");
    display.print(time_remaining); 

  } while (display.nextPage());
}






