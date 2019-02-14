
#include <FastLED.h>

#define LED_PIN1    8
#define LED_PIN2    9
#define LED_PIN3    10
#define NUM_LEDS    6 //Number of LEDs on current strips
CRGB strip1[NUM_LEDS];
CRGB strip2[NUM_LEDS];
CRGB strip3[NUM_LEDS];

#define BRIGHTNESS  40

//How many seconds to show each temp before switching
#define DISPLAYTIME 20 
//How many seconds to show black in between switches
#define BLACKTIME 3

#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky

void setup() {
  //Setting up all LED strips where need to specify the chipset, the pin, and the color order
  //as well as brightness

  Serial.begin(9600);

  FastLED.addLeds<WS2812, 8, GRB>(strip1, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );

  FastLED.addLeds<WS2812, 9, GRB>(strip2, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );

  FastLED.addLeds<WS2812, 10, GRB>(strip3, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );
}

char input = 0; //input bit for Serial monitor 

void loop() {
  int l;
  int i;
  
  if (Serial.available() > 0) {
    input = Serial.read(); //get input from serial monitor

    //--------------Alternating Red/Green-------------------//
    if (input == '1') {
      Serial.print("Alternating RG \n");
      for (l = 0; l < 20; l++) {
  
        for (i = 0; i < (NUM_LEDS - 1); i++) {
          strip1[i] = CRGB::Red; FastLED.show();
          strip1[i+1] = CRGB::Green; FastLED.show();
          strip2[i] = CRGB::Red; FastLED.show();
          strip2[i+1] = CRGB::Green; FastLED.show();
          strip3[i] = CRGB::Red; FastLED.show();
          strip3[i+1] = CRGB::Green; FastLED.show();
          i++;
        }
  
        FastLED.delay(150);
    
        for (i = 0; i < (NUM_LEDS - 1); i++) {
          strip1[i] = CRGB::Green; FastLED.show();
          strip1[i+1] = CRGB::Red; FastLED.show();
          strip2[i] = CRGB::Green; FastLED.show();
          strip2[i+1] = CRGB::Red; FastLED.show();
          strip3[i] = CRGB::Green; FastLED.show();
          strip3[i+1] = CRGB::Red; FastLED.show();
          i++;
        }
        FastLED.delay(150);
      }
    }

    //-------------Alternating Blue/Yellow---------//
    else if (input == '2') {
      Serial.print("Alternating BY  \n");
      for (l = 0; l < 20; l++) {
        for (i = 0; i < (NUM_LEDS - 1); i++) {
        strip1[i] = CRGB::Blue; FastLED.show();
        strip1[i+1] = CRGB::Yellow; FastLED.show();
        strip2[i] = CRGB::Blue; FastLED.show();
        strip2[i+1] = CRGB::Yellow; FastLED.show();
        strip3[i] = CRGB::Blue; FastLED.show();
        strip3[i+1] = CRGB::Yellow; FastLED.show();
        i++;
        }
    
        FastLED.delay(150);
    
        for (i = 0; i < (NUM_LEDS - 1); i++) {
          strip1[i] = CRGB::Yellow; FastLED.show();
          strip1[i+1] = CRGB::Blue; FastLED.show();
          strip2[i] = CRGB::Yellow; FastLED.show();
          strip2[i+1] = CRGB::Blue; FastLED.show();
          strip3[i] = CRGB::Yellow; FastLED.show();
          strip3[i+1] = CRGB::Blue; FastLED.show();
          i++;
        }
        FastLED.delay(150);
      }   
    }
//---------------------Running LED no wave-----------------//
    else if (input == '3') {
      Serial.print("Running LED no wave\n");

      for (l = 0; l < 20; l++) {
        for (i = 0; i < NUM_LEDS; i++) {
          strip1[i] = CRGB::Blue; FastLED.show();
          strip2[i] = CRGB::Blue; FastLED.show();
          strip3[i] = CRGB::Blue; FastLED.show();
          strip1[i] = CRGB::White; FastLED.show();
          strip2[i] = CRGB::White; FastLED.show();
          strip3[i] = CRGB::White; FastLED.show();delay(100);
          strip1[i] = CRGB::Red; FastLED.show();
          strip2[i] = CRGB::Red; FastLED.show();
          strip3[i] = CRGB::Red; FastLED.show();
        }
  
        for (i = 0; i < NUM_LEDS; i++) {
          strip1[i] = CRGB::Red; FastLED.show();
          strip2[i] = CRGB::Red; FastLED.show();
          strip3[i] = CRGB::Red; FastLED.show();
          strip1[i] = CRGB::White; FastLED.show();
          strip2[i] = CRGB::White; FastLED.show();
          strip3[i] = CRGB::White; FastLED.show();delay(100);
          strip1[i] = CRGB::Blue; FastLED.show();
          strip2[i] = CRGB::Blue; FastLED.show();
          strip3[i] = CRGB::Blue; FastLED.show();
        }
      }
    }
//------------------------Running LED wave----------------//
    else if (input == '4') {
      Serial.print("Running LED with wave\n");

      for (l = 0; l < 20; l++) {
        for (i = 0; i < NUM_LEDS; i++) {
          strip1[i] = CRGB::Blue; FastLED.show();
          strip1[i] = CRGB::White; FastLED.show();delay(70);
          strip1[i] = CRGB::Red; FastLED.show();
        }
        for (i = 0; i < NUM_LEDS; i++) {
          strip2[i] = CRGB::Blue; FastLED.show();
          strip2[i] = CRGB::White; FastLED.show();delay(70);
          strip2[i] = CRGB::Red; FastLED.show();
        }
        for (i = 0; i < NUM_LEDS; i++) {
          strip3[i] = CRGB::Blue; FastLED.show();
          strip3[i] = CRGB::White; FastLED.show();delay(70);
          strip3[i] = CRGB::Red; FastLED.show();
        }

        //reverse part
        
        for (i = 0; i < NUM_LEDS; i++) {
          strip1[i] = CRGB::Red; FastLED.show();
          strip1[i] = CRGB::White; FastLED.show();delay(70);
          strip1[i] = CRGB::Blue; FastLED.show();
        }
        for (i = 0; i < NUM_LEDS; i++) {
          strip2[i] = CRGB::Red; FastLED.show();
          strip2[i] = CRGB::White; FastLED.show();delay(70);
          strip2[i] = CRGB::Blue; FastLED.show();
        }
        for (i = 0; i < NUM_LEDS; i++) {
          strip3[i] = CRGB::Red; FastLED.show();
          strip3[i] = CRGB::White; FastLED.show();delay(70);
          strip3[i] = CRGB::Blue; FastLED.show();
        }
      }
    }
//--------------------Rainbow------------------//
    else if (input == '5') {
      Serial.print("Rainbow \n");

      for (l = 0; l < 2500; l++) {
        static uint8_t starthue = 0;
        fill_rainbow( strip1, NUM_LEDS, --starthue, 20);
        fill_rainbow( strip2, NUM_LEDS, --starthue, 20);
        fill_rainbow( strip3, NUM_LEDS, --starthue, 20);
      
        // Choose which 'color temperature' profile to enable.
        uint8_t secs = (millis() / 1000) % (DISPLAYTIME * 2);
      
        // Black out the LEDs for a few secnds between color changes
        // to let the eyes and brains adjust
        if( (secs % DISPLAYTIME) < BLACKTIME) {
          memset8( strip1, 0, NUM_LEDS * sizeof(CRGB)); delay(100);
          memset8( strip2, 0, NUM_LEDS * sizeof(CRGB)); delay(100);
          memset8( strip3, 0, NUM_LEDS * sizeof(CRGB)); delay(100);
        }
        FastLED.show();
        FastLED.delay(12);
      }
    }
  }
  
  
  FastLED.delay(200);
}
