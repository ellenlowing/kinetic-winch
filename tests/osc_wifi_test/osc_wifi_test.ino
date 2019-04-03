#include <ArduinoOSC.h>

// WiFi stuff
const char* ssid = "BU Guest (unencrypted)";
//const char* pwd = "your-password";
//const IPAddress ip(10, 192, 224, 201);
//const IPAddress gateway(10, 192, 224, 1);
//const IPAddress subnet(255, 255, 240, 0);

// for ArduinoOSC
OscWiFi osc;
const char* host = "155.41.48.193";
const int recv_port = 5000;
const int send_port = 12000;

void onOscReceived(OscMessage& m)
{
    Serial.print("callback : ");
    Serial.print(m.ip()); Serial.print(" ");
    Serial.print(m.port()); Serial.print(" ");
    Serial.print(m.size()); Serial.print(" ");
    Serial.print(m.address()); Serial.print(" ");
    Serial.print(m.arg<int>(0)); Serial.print(" ");
    Serial.print(m.arg<float>(1)); Serial.print(" ");
    Serial.print(m.arg<String>(2)); Serial.println();
}

void setup()
{
    Serial.begin(115200);

    // WiFi stuff
    WiFi.begin(ssid);
//    WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); }

    Serial.print("WiFi connected, IP = "); Serial.println(WiFi.localIP());

    // ArduinoOSC
    osc.begin(recv_port);
    
    osc.subscribe("/1/*", [](OscMessage& m)
    {
        String addr = m.address();
        String tag = addr.substring(3);
        float val = m.arg<float>(0);
        
        if( tag.equals("preset1") ) {
          Serial.println("preset1");
        } else if ( tag.equals("preset2") ) {
          Serial.println("preset2");
        } else if ( tag.equals("preset3") ) {
          Serial.println("preset3");
        } else if ( tag.equals("preset4") ) {
          Serial.println("preset4");
        } else if ( tag.equals("preset5") ) {
          Serial.println("preset5");
        } else if ( tag.equals("speed1") ) {
          // float value from 0.0 to 10.0
          Serial.println("speed1");
        } else if ( tag.equals("speed2") ) {
          Serial.println("speed2");
        } else if ( tag.equals("toggle1") ) {
          // 1: pause, 0: play
          Serial.println("toggle1");
        } else {
          Serial.println("others");
        }
        
    });
}

void loop()
{
    osc.parse(); // should be called
}
