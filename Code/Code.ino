// ###############################################################################################################################
// #
// # Code for "HAL 9000 Apple TV remote holder " project: https://www.printables.com/de/model/310576-hal-9000-apple-tv-remote-holder
// #
// # Code by https://github.com/AWSW-de
// #
// # Released under license: GNU General Public License v3.0: https://github.com/AWSW-de/HAL-9000-Apple-TV-remote-holder/blob/main/LICENSE
// #
// ###############################################################################################################################
/*
 _|    _|    _|_|    _|              _|_|      _|      _|      _|          _|_|                        _|                _|_|_|_|_|  _|      _|      
 _|    _|  _|    _|  _|            _|    _|  _|  _|  _|  _|  _|  _|      _|    _|  _|_|_|    _|_|_|    _|    _|_|            _|      _|      _|      
 _|_|_|_|  _|_|_|_|  _|              _|_|_|  _|  _|  _|  _|  _|  _|      _|_|_|_|  _|    _|  _|    _|  _|  _|_|_|_|          _|      _|      _|      
 _|    _|  _|    _|  _|                  _|  _|  _|  _|  _|  _|  _|      _|    _|  _|    _|  _|    _|  _|  _|                _|        _|  _|        
 _|    _|  _|    _|  _|_|_|_|      _|_|_|      _|      _|      _|        _|    _|  _|_|_|    _|_|_|    _|    _|_|_|          _|          _|          
                                                                                   _|        _|                                                      
                                                 _|                    _|          _|      _|_|      _|                                              
 _|  _|_|    _|_|    _|_|_|  _|_|      _|_|    _|_|_|_|    _|_|        _|_|_|      _|_|    _|    _|_|_|    _|_|    _|  _|_|                          
 _|_|      _|_|_|_|  _|    _|    _|  _|    _|    _|      _|_|_|_|      _|    _|  _|    _|  _|  _|    _|  _|_|_|_|  _|_|                              
 _|        _|        _|    _|    _|  _|    _|    _|      _|            _|    _|  _|    _|  _|  _|    _|  _|        _|                                
 _|          _|_|_|  _|    _|    _|    _|_|        _|_|    _|_|_|      _|    _|    _|_|    _|    _|_|_|    _|_|_|  _|                                
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
#define N_PIXELS 1  // Number of pixels
#define LED_PIN 32  // NeoPixel LED strand is connected to this pin
//#define BUTTON_PIN_BITMASK3334 0x600000000 // 2^33 + 2^34 in hex
#define BUTTON_PIN_1 33
#define BUTTON_PIN_2 34
#define BUTTON_PIN_BITMASK1 0x200000000  // 2^33 in hex
#define BUTTON_PIN_BITMASK2 0x400000000  // 2^34 in hex
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ###############################################################################################################################
// # LED default settings:
// ###############################################################################################################################
int minintesiity = 3;    // Default LED min intensity (0..255)
int maxintensity = 192;  // Default LED max intensity (0..255)
int breath_time = 15;    // Time between "breath" actions


//#########################################################################################
RTC_DATA_ATTR int bootCount = 0;
//#########################################################################################
/*
  Method to print the reason by which ESP32
  has been awaken from sleep
*/
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1:
      {
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK2, ESP_EXT1_WAKEUP_ANY_HIGH);
        for (int i = 0; i < 6; i++) {
          for (int a = minintesiity; a < maxintensity; a++) {
            strip.setBrightness(a);
            show_glow(strip.Color(255, 0, 0), breath_time);
          }
          for (int b = maxintensity; b >= minintesiity; b--) {
            strip.setBrightness(b);
            show_glow(strip.Color(255, 0, 0), breath_time);
          }
        }
        break;
      }
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default:
      {
        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK1, ESP_EXT1_WAKEUP_ALL_LOW);
        break;
      }
  }
}


// ###############################################################################################################################
// # Setup function to run at startup:
// ###############################################################################################################################
void setup() {
  Serial.begin(115200);
  delay(500);  //Take some time to open up the Serial Monitor
  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  pinMode(BUTTON_PIN_1, INPUT_PULLUP);  // pinMode(buttonPin, INPUT with PULLUP resistor integrated in the ESP);
  digitalWrite(BUTTON_PIN_1, HIGH);     // initialize the buttonPin as output
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);  // pinMode(buttonPin, INPUT with PULLUP resistor integrated in the ESP);
  digitalWrite(BUTTON_PIN_2, HIGH);     // initialize the buttonPin as output

  strip.begin();
  strip.setBrightness(minintesiity);
  if (bootCount == 3) {
    Serial.println("Neustart!!!: " + String(bootCount));
    ESP.restart();
  }
  //Print the wakeup reason for ESP32
  print_wakeup_reason();
  //Go to sleep now
  Serial.println("Going to sleep now");
  delay(1000);
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}


// ###############################################################################################################################
// # Loop funtion to run all the time:
// ###############################################################################################################################
void loop() {
  // not used to save power
}


// ###############################################################################################################################
// # Outside triangle function:
// ###############################################################################################################################
void show_glow(uint32_t color, int delaywait) {
  strip.setPixelColor(0, 255, 0, 0);  // RED LED
  strip.show();
  delay(delaywait);
}