#include <pthread.h>
#include <WiFi.h>
#include <ArduinoOSC.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <NeoPixelBrightnessBus.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

// Thread variables
pthread_t threads[3];

// WiFi variables
const char *ssid = "BU Guest (unencrypted)";
OscWiFi osc;
const char * host = "10.192.228.184";
const int recv_port = 2000;
const int send_port = 2111;
String oscMessage;

// LED variables
const uint16_t numPixels = 3;
const uint16_t ledPin = 21;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(numPixels, ledPin);
int r = 0;
int g = 0;
int b = 0;

// Motor variables
const int ENABLE = 12;
const int STEP = 15;
const int DIR = 33;
const int MS1 = 35;
const int MS2 = 32;
const int MS3 = 14;
const int STEPS_PER_ROTATION = 200;
int speed = 0;
int direction = 0;
int steps = STEPS_PER_ROTATION;

void * getOscMessage ( void * ) {
  while(true) {
    osc.parse();

    char buf [5];
    String str = oscMessage.substring(0, 3); // red
    str.toCharArray(buf, 4);
    sscanf(buf, "%d", &r);
    str = oscMessage.substring(3, 6); // blue
    str.toCharArray(buf, 4);
    sscanf(buf, "%d", &b);
    str = oscMessage.substring(6, 9); // green
    str.toCharArray(buf, 4);
    sscanf(buf, "%d", &g);
    str = oscMessage.substring(9, 12); // speed
    str.toCharArray(buf, 4);
    sscanf(buf, "%d", &speed);
    speed = map(speed, 0, 10, 10000, 4000);
    speed = constrain(speed, 4000, 10000);
    str = oscMessage.substring(12); // direction
    str.toCharArray(buf, 2);
    sscanf(buf, "%d", &direction);
    Serial.println(direction);

    delay(10);
  }
}

// LED control
void* ledControl(void *) {
  while(1) {
    for(int i = 0; i < numPixels; i++) {
      RgbColor color(r, g, b);
      strip.SetPixelColor(i, color);
    }
    strip.Show();
    delay(10);
  }
}

// Motor control
void * motorControl(void *) {
  while(1) {
    switch (direction) {
      case 0: {
        steps = STEPS_PER_ROTATION;
        digitalWrite(DIR, LOW);
        break;
      }

      case 1: {
        steps = STEPS_PER_ROTATION;
        digitalWrite(DIR, HIGH);
        break;
      }

      case 2: {
        steps = 0;
        break;
      }

      default: {
        Serial.println("wrong direction");
      }
    }

    stepNow(steps);
  }
}

void stepNow(int totalSteps) {
  for (int i = 0; i < totalSteps; ++i) {
    if(direction == 2) break;
    digitalWrite(STEP, HIGH);
    delayMicroseconds(speed);
    digitalWrite(STEP, LOW);
    delayMicroseconds(speed);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); }
  Serial.print("WiFi connected, IP = "); Serial.println(WiFi.localIP());

  // osc setup
  osc.begin(recv_port);
  osc.subscribe("/client/0", [](OscMessage& m)
  {
    oscMessage = m.arg<String>(0);
  });

  // led setup
  strip.Begin();
  strip.Show();

  // motor setup
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);

  // threads setup
  int osc_thread = pthread_create(&threads[0], NULL, getOscMessage, NULL);
  int led_thread = pthread_create(&threads[1], NULL, ledControl, NULL);
  int motor_thread = pthread_create(&threads[2], NULL, motorControl, NULL);
  if(osc_thread) Serial.println("Error setting up osc thread");
  if(led_thread) Serial.println("Error setting up led thread");
  if(motor_thread) Serial.println("Error setting up motor thread");
}

void loop() { }
