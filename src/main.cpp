#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

// Display connections - adjust according to your wiring
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

void updateDisplay(); 


void setup() {
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
    display.println("FireBeetle ESP32");
    
    // Display sensor info (example)
    display.setCursor(10, 50);
    display.print("Temp: ");
    display.print(random(20, 30));  // Simulated data
    display.println(" C");
    
    display.setCursor(10, 80);
    display.print("Humidity: ");
    display.print(random(40, 60));  // Simulated data
    display.println(" %");
    
    // Display counter
    display.setCursor(10, 120);
    display.print("Updates: ");
    display.print(counter);
    
    // Display footer
    display.setCursor(10, 180);
    display.println("Waveshare 1.54\" e-Paper");
  } while (display.nextPage());
}