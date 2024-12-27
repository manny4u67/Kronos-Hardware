#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>
#include <FastLED.h>
#include <AS5600.h>

// OLED Display Configuration
#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define SDA0_Pin 17   // ESP32 SDA PIN
#define SCL0_Pin 18   // ESP32 SCL PIN
// ADS1115 Configurations
Adafruit_ADS1115 ads1_0x49;  // First ADS1115 at 0x49
Adafruit_ADS1115 ads2_0x48;  // Second ADS1115 at 0x48


// AS5600 Hall Effect Sensor
AS5600 as5600;

// LED Strip Configurations
#define LED_PIN_75  48
#define LED_PIN_6   35
#define NUM_LEDS_75 75
#define NUM_LEDS_6  6
CRGB leds_75[NUM_LEDS_75];
CRGB leds_6[NUM_LEDS_6];

void setup() {
  // Initialize Serial Communication
  Serial.begin(115200);

  // Initialize I2C
  Wire.begin(SCL0_Pin, SDA0_Pin );  // SCL = 17, SDA = 18
  // Initialize OLED Display
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);

  // Initialize ADS1115 Modules
  if (!ads1_0x49.begin(0x49)) {
    Serial.println("Failed to initialize ADS1115 at 0x49");
    for(;;);
  }
  if (!ads2_0x48.begin(0x48)) {
    Serial.println("Failed to initialize ADS1115 at 0x48");
    for(;;);
  }

  // Initialize AS5600 Hall Effect Sensor
  as5600.begin();

  // Initialize LED Strips
  FastLED.addLeds<WS2812B, LED_PIN_75, GRB>(leds_75, NUM_LEDS_75);
  FastLED.addLeds<WS2812B, LED_PIN_6, GRB>(leds_6, NUM_LEDS_6);
  FastLED.setBrightness(25);

    // LED Strip Animation
  rainbowCycle(leds_75, NUM_LEDS_75);
  solidColor(leds_6, NUM_LEDS_6, CRGB::Blue);
}

void loop() {
  // Read AS5600 Hall Effect Sensor
  uint16_t angle = as5600.rawAngle();

  // Read ADS1115 Channels
  int16_t ads1_ch0 = ads1_0x49.readADC_SingleEnded(0);
  int16_t ads1_ch1 = ads1_0x49.readADC_SingleEnded(1);
  int16_t ads1_ch2 = ads1_0x49.readADC_SingleEnded(2);
  int16_t ads2_ch0 = ads2_0x48.readADC_SingleEnded(0);
  int16_t ads2_ch1 = ads2_0x48.readADC_SingleEnded(1);
  int16_t ads2_ch2 = ads2_0x48.readADC_SingleEnded(2);

  // Clear Display
  oled.clearDisplay();
  oled.setCursor(0, 0);

  // Display Measurements
  oled.println(F("Sensor Readings:"));
  oled.print(F("Hall Angle: "));
  oled.println(angle);
  oled.print(F("ADS1 Ch0: "));
  oled.println(ads1_ch0);
  oled.print(F("ADS1 Ch1: "));
  oled.println(ads1_ch1);
  oled.print(F("ADS1 Ch2: "));
  oled.println(ads1_ch2);
  oled.print(F("ADS2 Ch0: "));
  oled.println(ads2_ch0);
  oled.print(F("ADS2 Ch1: "));
  oled.println(ads2_ch1);
  oled.print(F("ADS2 Ch1: "));
  oled.println(ads2_ch2);
  oled.display();



  delay(1);  // Update interval
}

void rainbowCycle(CRGB* leds, int numLeds) {
  for(int i = 0; i < numLeds; i++) {
    leds[i] = CHSV(255, 255, 255);
  }
  FastLED.show();
}

void solidColor(CRGB* leds, int numLeds, CRGB color) {
  for(int i = 0; i < numLeds; i++) {
    leds[i] = color;
  }
  FastLED.show();
}