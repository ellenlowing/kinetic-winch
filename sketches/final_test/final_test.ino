
#include <pthread.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <string.h>
#include <NeoPixelBrightnessBus.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#define ENABLE 12
#define STEP 15
#define DIR 33
#define MS1 35
#define MS2 32
#define MS3 14
#define STEPS_PER_ROTATION 200

// Thread variables
pthread_t threads[2];

// Wifi variables
const char* ssid = "BU Guest (unencrypted)";
WiFiServer wifiServer(8090);
String entry = "\0";
String prevEntry = "\0";

// Motor variables
int speed = 4000; // 
int brightness = 0;
int windMode = 0;

// LED variables
const uint16_t numPixels = 3;
const uint16_t ledPin = 21;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(numPixels, ledPin);
int r, g, b;

void * clientReceive(void *) {
  while(1) {
    WiFiClient client = wifiServer.available();
    if (client) {
      Serial.println("Connected to wifi server");
  
      while (client.connected()) {
  
        while (client.available() > 0) {
          char c = client.read();
          client.write(c);
          Serial.print(c);
          entry.concat(c);
        }

        if(entry.length() != 13) {
          entry = prevEntry;
        }
        
        delay(10);
      }
  
      // Parse the data to extract the inputs
   
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& obj = jsonBuffer.createObject();
      parsing(obj);
      r = getR(obj);
      b = getB(obj);
      g = getG(obj);
      int rpm = getRPM(obj);
      windMode = getDir(obj);
      prevEntry = entry;
      entry = "\0";
      jsonBuffer.clear();

      client.stop();
    } 
    delay(10);
  }
}

void parsing(JsonObject& obj ) {
  Serial.print("Entry: ");
  Serial.println(entry);

  obj["r"] = entry.substring(0, 3);
  obj["b"] = entry.substring(3, 6);
  obj["g"] = entry.substring(6, 9);
  obj["rpm"] = entry.substring(9, 12);
  obj["dir"] = entry.substring(12);

//  obj.prettyPrintTo(Serial);

}

int getR(JsonObject& obj ) {
  int r = atoi(obj["r"]);
  return r;
}

int getB(JsonObject& obj ) {
  int b = atoi(obj["b"]);
  return b;
}

int getG(JsonObject& obj ) {
  int g = atoi(obj["g"]);
  return g;
}

int getRPM(JsonObject& obj ) {
  int rpm = atoi(obj["rpm"]);
  return rpm;
}

int getDir(JsonObject& obj ) {
  int dir = atoi(obj["dir"]);
  return dir;
}

void* motorControl(void *) {
  while(1) {
    if (windMode == 0) {
      stepUp();
    } else {
      stepDown();
    }
  }
}

void stepNow(int totalSteps) {
  for (int i = 0; i < totalSteps; ++i) {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(speed);
    digitalWrite(STEP, LOW);
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

void setup() {
  Serial.begin(115200);

  // Wifi config
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
  wifiServer.begin();

  // Motor config
//  Serial.println("Enter 1 to unwind cable, 0 to wind cable");
//  pinMode(STEP, OUTPUT);
//  pinMode(DIR, OUTPUT);
//  pinMode(MS1, OUTPUT);
//  pinMode(MS2, OUTPUT);
//  pinMode(MS3, OUTPUT);

  // LED config
  strip.Begin();
  strip.Show();

  // Thread config
//  int thread0 = pthread_create(&threads[0], NULL, motorControl, NULL);
  int thread0 = pthread_create(&threads[0], NULL, ledControl, NULL);
  int thread1 = pthread_create(&threads[1], NULL, clientReceive, NULL);
  
  if (thread0) {
    Serial.println("thread 0 has error");
  }
  if (thread1) {
    Serial.println("thread 1 has error");
  }
}

void loop() {

}
