// LED
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#define LED_PIN 36
#define NUM_LEDS 3
CRGB leds[NUM_LEDS];

// A4988 driver test routine
#define ENABLE 6
#define STEP 5
#define DIR 3
#define STEPS_PER_ROTATION 200

int speed = 800;
int brightness = 0;
int windMode = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter 1 to unwind cable, 0 to wind cable");

  // Motor
  pinMode(STEP,OUTPUT);
  pinMode(DIR,OUTPUT);

  // LED
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}


void stepNow(int totalSteps) {
  for(int i=0;i<totalSteps;++i) {
    digitalWrite(STEP,HIGH);
    delayMicroseconds(speed);
    digitalWrite(STEP,LOW);
    delayMicroseconds(speed);
  }
}

// winding cable back up
void stepUp() {
  Serial.println("winding cable up");
  digitalWrite(DIR, LOW);
  stepNow(STEPS_PER_ROTATION);
}

// unwinding cable
void stepDown() {
  Serial.println("unwinding cable");
  digitalWrite(DIR, HIGH);
  stepNow(STEPS_PER_ROTATION);
}

void loop() {
  if(Serial.available() > 0) {
    char ch = Serial.read();
    int ch_int = ch - '0';
    if(ch_int >= 0) windMode = ch - '0';
    Serial.println(windMode);
  }

  // Motor
  if(windMode == 0) {
    stepUp();
  } else {
    stepDown();
  }

  spinningRainbow();
  
  FastLED.setBrightness(  64 );
  FastLED.show();
}

void spinningRainbow() {
  // variable used for the initial hue of the rainbow
  // we start it out at 0
  // but since it's static, it'll keep it's value as we change it
  static byte initialHue = 0;
  
  // increase the hue by 1 each time
  initialHue = initialHue + 1;
  
  // the amount we want the hue to change between each LED
  // by dividing the number of hues (255), by the number of LEDs,
  // this code makes each LED a different color
  // and covers the entire rainbow spectrum (red, orange, yellow, green, blue, indigo, violet)
  byte changeInHue = 255 / NUM_LEDS;
  
  // use FastLED to fill the LEDs with a rainbow
  fill_rainbow(leds, NUM_LEDS, initialHue, changeInHue);
}
