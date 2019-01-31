#include <FastLED.h>
#define LED_PIN1     8
#define LED_PIN2     9
#define LED_PIN3    10
#define NUM_LEDS     6 //how many LEDs are on the strip you're currently using
CRGB leds[NUM_LEDS];

#define bcolor    Blue
#define rcolor    White
#define lcolor    Red

void setup() {
  FastLED.addLeds<WS2812, LED_PIN1, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN2, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN3, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness( 16 );
}

void loop() {

  int i;

  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::bcolor; FastLED.show();
    leds[i] = CRGB::rcolor; FastLED.show();delay(150);
    leds[i] = CRGB::lcolor; FastLED.show();
  }

  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::lcolor; FastLED.show();
    leds[i] = CRGB::rcolor; FastLED.show();delay(150);
    leds[i] = CRGB::bcolor; FastLED.show();
  }
}
