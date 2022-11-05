// ###############################################################################################################################
// #
// # Code for "HAL 9000 knob" project: https://www.printables.com/de/model/310576-hal-9000-knob
// #
// # Code by https://github.com/AWSW-de with the use of some code from Adafruit examples. See notes at the end of the code too.
// #
// # Released under license: GNU General Public License v3.0: https://github.com/AWSW-de/HAL-9000-knob/blob/main/LICENSE
// #
// ###############################################################################################################################
/*
       _|    _|    _|_|    _|              _|_|      _|      _|      _|        _|                            _|        
       _|    _|  _|    _|  _|            _|    _|  _|  _|  _|  _|  _|  _|      _|  _|    _|_|_|      _|_|    _|_|_|    
       _|_|_|_|  _|_|_|_|  _|              _|_|_|  _|  _|  _|  _|  _|  _|      _|_|      _|    _|  _|    _|  _|    _|  
       _|    _|  _|    _|  _|                  _|  _|  _|  _|  _|  _|  _|      _|  _|    _|    _|  _|    _|  _|    _|  
       _|    _|  _|    _|  _|_|_|_|      _|_|_|      _|      _|      _|        _|    _|  _|    _|    _|_|    _|_|_|                                                                                                               
*/


// ###############################################################################################################################
// # Includes:
// #
// # You will need to add the following libraries to your Arduino IDE to use the project:
// # - Adafruit NeoPixel              // by Adafruit:                     https://github.com/adafruit/Adafruit_NeoPixel
// #
// ###############################################################################################################################
#include <Adafruit_NeoPixel.h>  // Used to drive the NeoPixel LEDs


// ###############################################################################################################################
// #
// # Code version:
// #
// ###############################################################################################################################
String code_version = "V1.0.0";


// ###############################################################################################################################
// # Hardware settings:
// ###############################################################################################################################
#define N_PIXELS 13  // Number of pixels in strand
#define LED_PIN D2   // NeoPixel LED strand is connected to this pin
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);


// ###############################################################################################################################
// # LED default settings:
// ###############################################################################################################################
int minintesiity = 3;   // Default LED min intensity (0..255)
int maxintensity = 42;  // Default LED max intensity (0..255) - 42 is always the correct intensity, Dave! ;-)
int breath_time = 75;   // Time between "breath" actions


// ###############################################################################################################################
// # Setup function to run at startup:
// ###############################################################################################################################
void setup() {
  strip.begin();
  strip.setBrightness(minintesiity);
}


// ###############################################################################################################################
// # Loop funtion to run all the time:
// ###############################################################################################################################
void loop() {
  for (int a = minintesiity; a < maxintensity; a++) {
    strip.setBrightness(a);
    show_glow(strip.Color(255, 0, 0), breath_time);
  }
  for (int b = maxintensity; b >= minintesiity; b--) {
    strip.setBrightness(b);
    show_glow(strip.Color(255, 0, 0), breath_time);
  }
}


// ###############################################################################################################################
// # Outside triangle function:
// ###############################################################################################################################
void show_glow(uint32_t color, int delaywait) {
  int myArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
  for (int element : myArray) {
    strip.setPixelColor(element, color);  // Red LED 1-12
  }
  strip.setPixelColor(12, 255, 255, 0);   // Yellow LED 13
  strip.show();
  delay(delaywait);
}