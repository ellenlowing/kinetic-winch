#include <FastLED.h>
#define LED_PIN1     8
#define LED_PIN2     7
#define NUM_LEDS     6 //how many LEDs are on the strip you're currently using
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812, LED_PIN1, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN2, GRB>(leds, NUM_LEDS);
}

void loop() {

  int i;

  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White; FastLED.show();
    leds[i] = CRGB::Black; FastLED.show();delay(300);
  }
  
}
