#include <NeoPixelBus.h>

const uint16_t numPixels = 3;
const uint16_t pin = 21;
const uint16_t maxSaturation = 128;
//uint16_t saturation = 0;

float rate = 0.005;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(numPixels, pin);

void setup() {
  Serial.begin(115200);
  strip.Begin();
  strip.Show();  

}

void loop() {
  for(int i = 0; i < numPixels; i++) {
    RgbColor color(fade(rate, i*10), 0, 0);
    strip.SetPixelColor(i, color);
  }
  strip.Show();
  Serial.println(fade(rate, 0));
}

uint16_t fade(float rate, float offset) {
  uint16_t brightness = 0;
  brightness = (sin(millis() * rate + offset) + 1) * maxSaturation / 2;
  return brightness;
}

