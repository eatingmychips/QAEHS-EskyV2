#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <esp_sleep.h>
#include "RTClib.h"
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <EEPROM.h>
#include <motor_functions.h>



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

uint16_t counter = 0;


// Setup IR Receiver 
int RECV_PIN = 6; // Define input pin on arduino 
unsigned long IRCode = 0; // Initialise IRCode (to be received from IR Remote)
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
void updateDisplay(); 

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
  
  // Display initial content
  updateDisplay();
}

void loop() {
  delay(5000);  // Update every 5 seconds
  counter++;
  updateDisplay();
}

void updateDisplay() {
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
    display.println("Ebony Is So Cute");
    
    // Display sensor info (example)
    display.setCursor(10, 50);
    display.print("Ebony's But / 10: ");
    display.print("10");  // Simulated data
    display.println(" C");
    
    display.setCursor(10, 80);
    display.print("Ebonys Face / 10");
    display.print("10");  // Simulated data
    display.println(" %");
    
    // Display counter
    display.setCursor(10, 120);
    display.print("Updates: ");
    display.print(counter);
    
    // Display footer
    display.setCursor(10, 180);
    display.println("Waveshare 1.54\" e-Paper");
  } while (display.nextPage());

  intialise_pump(1, duty_cycle);
}






