#include <FastLED.h>
#define LED_PIN1     8
#define LED_PIN2     9
#define LED_PIN3    10
#define NUM_LEDS     6 //how many LEDs are on the strip you're currently using
CRGB leds[NUM_LEDS];

#define bcolor    Red
#define rcolor    Green


void setup() {
  FastLED.addLeds<WS2812, LED_PIN1, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN2, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN3, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness( 64 );
}

void loop() {

  int i;

  for (i = 0; i < (NUM_LEDS - 1); i++) {
    leds[i] = CRGB::bcolor; FastLED.show();
    leds[i+1] = CRGB::rcolor; FastLED.show();
    i++;
  }

  FastLED.delay(150);

  for (i = 0; i < (NUM_LEDS - 1); i++) {
    leds[i] = CRGB::rcolor; FastLED.show();
    leds[i+1] = CRGB::bcolor; FastLED.show();
    i++;
  }

  FastLED.delay(200);
}
