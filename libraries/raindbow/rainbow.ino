#include <FastLED.h>

#define LED_PIN     8

// Information about the LED strip itself
#define NUM_LEDS    6
#define CHIPSET     WS2811
#define COLOR_ORDER GRB
CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];

#define BRIGHTNESS  32

#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky

// How many seconds to show each temperature before switching
#define DISPLAYTIME 20
// How many seconds to show black between switches
#define BLACKTIME   3

void setup() {
  //delay( 500 );
  // so that colors can be more accurately rendered through the 'temperature' profiles
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );

  FastLED.addLeds<CHIPSET, 8, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );

  FastLED.addLeds<CHIPSET, 9, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );

  FastLED.addLeds<CHIPSET, 9, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );

  FastLED.addLeds<CHIPSET, 10, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );

  FastLED.addLeds<CHIPSET, 10, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );
}

void loop()
{
  // draw a generic, no-name rainbow
  static uint8_t starthue = 0;
  fill_rainbow( leds1, NUM_LEDS, --starthue, 20);

  // Choose which 'color temperature' profile to enable.
  uint8_t secs = (millis() / 1000) % (DISPLAYTIME * 2);
  if( secs < DISPLAYTIME) {
    FastLED.setTemperature( TEMPERATURE_1 ); // first temperature
    leds1[0] = TEMPERATURE_1; // show indicator pixel
  } else {
    FastLED.setTemperature( TEMPERATURE_2 ); // second temperature
    leds1[0] = TEMPERATURE_2; // show indicator pixel
  }

  // Black out the LEDs for a few secnds between color changes
  // to let the eyes and brains adjust
  if( (secs % DISPLAYTIME) < BLACKTIME) {
    memset8( leds1, 0, NUM_LEDS * sizeof(CRGB));
  }

  
  
  FastLED.show();
  FastLED.delay(2);
}



/*
 *   leds[0] = CRGB::Red; FastLED.show(); delay(25);
  leds[0] = CRGB::Blue; FastLED.show();delay(100);
  
  leds[1] = CRGB::Red; FastLED.show();delay(25);
  leds[1] = CRGB::Blue; FastLED.show();delay(100);
  
  leds[2] = CRGB::Red; FastLED.show(); delay(25);
  leds[2] = CRGB::Blue; FastLED.show(); delay(100);

  leds[3] = CRGB::Red; FastLED.show(); delay(25);
  leds[3] = CRGB::Blue; FastLED.show();delay(100);
  
  leds[4] = CRGB::Red; FastLED.show(); delay(25);
  leds[4] = CRGB::Blue; FastLED.show();delay(100);
  
  leds[5] = CRGB::Red; FastLED.show(); delay(25);
  leds[5] = CRGB::Blue; FastLED.show(); delay(100);
 */




//fade, random, noise, HSV to RGB, 
//void running_light(int delay) { 
//  // arguments: background light, running light, speed, size of running light
//}

//  leds[0] = CRGB(255, 0, 0);
//  FastLED.show();
//  delay(500);  
//  leds[1] = CRGB(0, 255, 0);
//  FastLED.show();
//  delay(500);
//  leds[2] = CRGB(0, 0, 255);
//  FastLED.show();
//  delay(500);
//  leds[0] = CRGB(150, 0, 255);
//  FastLED.show();
//  delay(500);
//  leds[1] = CRGB(255, 200, 20);
//  FastLED.show();
//  delay(500);
//  leds[2] = CRGB(85, 60, 180);
//  FastLED.show();
//  delay(500);
//  leds[0] = CRGB(50, 255, 20);
//  FastLED.show();
//  delay(500);
