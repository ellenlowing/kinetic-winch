#include <Wire.h>
#include <ArduinoJson.h>
#include <string.h>

StaticJsonBuffer<200> jsonBuffer;
JsonObject& obj = jsonBuffer.createObject();

String entry;

void getR() {
  char rd[4] = obj["r"];
  int r = atoi(rd);
  return r;
}

void getB() {
  char bl[4] = obj["b"];
  int b = atoi(bl);
  return b;
}

void getG() {
  char gr[4] = obj["g"];
  int g = atoi(gr);
  return g;
}

void getRPM() {
  char rm[4] = obj["rpm"];
  int rpm = atoi(rm);
  return rpm;
}

void getDir() {
  char d[4] = obj["dir"];
  int dir = atoi(d);
  return dir;
}

void setup() {
  Serial.begin(9600);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  delay(1000);           // start serial for output
}

void loop() {
  delay(1000);
  parsing();
  int r = getR();
  int b = getB();
  int g = getG();
  int rpm = getRPM();
  int dir = getDir();
  Serial.print("red: ");
  Serial.println(r);
  Serial.print("blue: ");
  Serial.println(b);
  Serial.print("green: ");
  Serial.println(g);
  Serial.print("speed: ");
  Serial.println(rpm);
  Serial.print("dir: ");
  Serial.println(dir);
}

int count = 0;

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  Serial.print("Bytes that we are receiving: ");
  count = Wire.available();
  Serial.println(count);
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
    entry.concat(c);
  }
}

void parsing() {
  Serial.print("Entry: ");
  Serial.println(entry);

  obj["r"] = entry.substring(0,3);
  obj["b"] = entry.substring(3,6);
  obj["g"] = entry.substring(6,9);
  obj["rpm"] = entry.substring(9,12);
  obj["dir"] = entry.substring(12);

  obj.prettyPrintTo(Serial);
  
}
