
#include <pthread.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <string.h>
#include <NeoPixelBrightnessBus.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <ArduinoOSC.h>

#define ENABLE 12
#define STEP 15
#define DIR 33
#define MS1 35
#define MS2 32
#define MS3 14
#define STEPS_PER_ROTATION 200

// Thread variables
pthread_t threads[3];

// Wifi variables
const char* ssid = "BU Guest (unencrypted)";
WiFiServer wifiServer(8099);//8090 for board 1, 8095 for board 2
String entry = "\0";
String prevEntry = "\0";

// OSC variables
OscWiFi osc;
const char* host = "10.192.228.184";
const int recv_port = 3000;
const int send_port = 3111;
String osc_str = "";

// Motor variables
int speed = 10000; // 4000 - 10000
int brightness = 0;
int windMode = 0;

// LED variables
const uint16_t numPixels = 3;
const uint16_t ledPin = 21;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(numPixels, ledPin);
int r, g, b;

void * clientReceive(void *) {
  while(1) {

      // osc
      osc.parse();
      parsing(osc_str);

      
//      r = getR(obj);
//      b = getB(obj);
//      g = getG(obj);
//      speed = map(getRPM(obj), 0, 10, 10000, 4000);
//      speed = constrain(speed, 4000, 10000);
//      windMode = getDir(obj);
//      prevEntry = entry;
//      entry = "\0";
//      jsonBuffer.clear();
      delay(10);
  }
}

void parsing( String s ) {
  String red = s.substring(0,3);
  char *r = red[0];
//  string blue = s.substring(3, 6);
//  tring green = s.substring(6, 9);
//  String rpm = s.substring(9, 12);
//  String dir = s.substring(12);

  
  int z = atoi(r);
//  g = atoi(;
//  b = blue.toInt();
//  speed = map(rpm.toInt(), 0, 10, 10000, 4000);
//  speed = constrain(speed, 4000, 10000);
//  windMode = dir.toInt();

  
      Serial.println("red " + red); 
      Serial.println(" r " + z);
//      Serial.println("green " + green); 
//      Serial.println(" g " + g);
//      Serial.println("blue " + blue); 
//      Serial.println(" b " + b);

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
//  Serial.println("winding cable up");
  digitalWrite(DIR, LOW);
  stepNow(STEPS_PER_ROTATION);
}

// unwinding cable
void stepDown() {
//  Serial.println("unwinding cable");
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
  while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); }
  Serial.print("WiFi connected, IP = "); Serial.println(WiFi.localIP());

  // ArduinoOSC
  osc.begin(recv_port);
  osc.subscribe("/client/1", [](OscMessage& m)
  {
//        Serial.print("client : ");
//        Serial.print(m.ip()); Serial.print(" ");
//        Serial.print(m.port()); Serial.print(" ");
//        Serial.print(m.size()); Serial.print(" ");
//        Serial.print(m.address()); Serial.print(" ");
//        Serial.print(m.arg<String>(0)); Serial.println();
      osc_str = m.arg<String>(0);
      Serial.println("msg: " + osc_str);
  });

  // Motor config
  Serial.println("Enter 1 to unwind cable, 0 to wind cable");
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);

  // LED config
  strip.Begin();
  strip.Show();

  // Thread config
  int thread0 = pthread_create(&threads[0], NULL, ledControl, NULL);
  int thread1 = pthread_create(&threads[1], NULL, clientReceive, NULL);
  int thread2 = pthread_create(&threads[2], NULL, motorControl, NULL);
  
  if (thread0) {
    Serial.println("thread 0 has error");
  }
  if (thread1) {
    Serial.println("thread 1 has error");
  }
  if(thread2) {
    Serial.println("thread 2 has error");
  }
}

void loop() {

}
