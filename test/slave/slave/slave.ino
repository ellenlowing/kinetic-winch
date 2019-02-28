#include <Wire.h>
#include <ArduinoJson.h>
#include <string.h>

StaticJsonBuffer<200> jsonBuffer;
JsonObject& obj = jsonBuffer.createObject();

String entry;

int getR() {
  int r = atoi(obj["r"]);
  return r;
}

int getB() {
  int b = atoi(obj["b"]);
  return b;
}

int getG() {
  int g = atoi(obj["g"]);
  return g;
}

int getRPM() {
  int rpm = atoi(obj["rpm"]);
  return rpm;
}

int getDir() {
  int dir = atoi(obj["dir"]);
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
  delay(5000);
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
