#include <WiFi.h>
#include <ArduinoJson.h>
#include <string.h>
 
const char* ssid = "BU Guest (unencrypted)";
//const char* password =  "smilingfamily";
 
WiFiServer wifiServer(8090);

StaticJsonBuffer<200> jsonBuffer;
JsonObject& obj = jsonBuffer.createObject();

String entry;
 
void setup() {
 
  Serial.begin(115200);
 
  delay(1000);
 
  WiFi.begin(ssid);
  
 // Connecting to WIFI
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
 
  wifiServer.begin();
}
 
void loop() {

 // Checking to see if wifi server is available 
  WiFiClient client = wifiServer.available();

  // If available, connect to the server and received the data that is being sent from the server side
  if (client) {
    Serial.println("Connected to wifi server");
 
    while (client.connected()) {
 
      while (client.available()>0) {
        char c = client.read();
        client.write(c);
        Serial.print(c);
        entry.concat(c);
      }
 
      delay(10);
    }
    
    // Parse the data to extract the inputs
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
 //   delay(5000);
    entry = "\0";
 
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
