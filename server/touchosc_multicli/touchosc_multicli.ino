#include <pthread.h>
#include <SPI.h>
#include <WiFi.h>
#include <ArduinoOSC.h>

/*
 * Timing variables
 */
unsigned long time0 = 0;
unsigned long time1 = 0;
unsigned long delay_time = 1000;
unsigned long dif;
int inc0 = 0;
int inc1 = 0;

/*
 * Communication to Winches variables
 */
//char msg[32];
//char *res;
char ssid[] = "BU Guest (unencrypted)"; //  your network SSID (name)
const uint16_t port0 = 8090;
const uint16_t port1 = 8099;
const char * host0 = "10.192.237.85";
const char * host1 = "10.192.234.231";
int status = WL_IDLE_STATUS;

/*
 * TouchOSC variables
 */
OscWiFi osc;
const char * oschost = "155.41.48.193";
const int osc_recv_port = 5000;
const int osc_send_port = 5111;
int preset = 0;
float motorSpeed = 0;
float ledSpeed = 0;
bool playing = true;
bool randomMode = false;
// for winches
const int osc_port0 = 2000;
const int osc_port1 = 3000;

/*
 * LED color variables
 */
const uint8_t maxSaturation = 128;
uint8_t r0 = 0;
uint8_t g0 = 0;
uint8_t b0 = 0;
uint8_t r1 = 0;
uint8_t g1 = 0;
uint8_t b1 = 0;
uint8_t globalR = 0;
uint8_t globalG = 0;
uint8_t globalB = 0;

/*
 * Motor variables
 */
int rpm = 0;
int dir = 0;

void setup() {
  // starting serial monitor
  Serial.begin(115200);

  // starting wifi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid);

    delay(5000);
  }
  Serial.println("Connected to wifi");
  Serial.print("WiFi connected, IP = "); Serial.println(WiFi.localIP());

  osc.begin(osc_recv_port);
  // Inidialize two threads for each client
  pthread_t threads[3];
  int thread0;
  int thread1;
  int thread2;
  thread0 = pthread_create(&threads[0], NULL, touchosc_server, NULL);
//  thread1 = pthread_create(&threads[1], NULL, connection0, NULL);
  thread2 = pthread_create(&threads[2], NULL, connection1, NULL);
  if (thread0) { Serial.println("An error has occurred thread0");}
//  if (thread1) { Serial.println("An error has occurred thread1");}
  if (thread2) { Serial.println("An error has occurred thread2 (touchosc)");}
 
}

/* 
 *  Connection with first client
 */
void *connection0(void *) {

  WiFiClient client;
  char msg[32];
  char *res;
  
  while( true ) {

      /*
     * Testing data to see if it is being sent and recveived
     * Inputting random values for inputs
     * Sending to client side as a string
     */

    if(millis() > (delay_time + time0)) {
      
      while (!client.connect(host0, port0)) {
        Serial.println("Connection to host 1 failed");
  
        delay(100);
      }
      
      // setting values for 
      float rate = (1.0 / (float)delay_time) * 5.0;   // 5.0 speeds up the fading
      float offset = 0;
      uint8_t r = fade(inc0, rate, offset); 
      uint8_t b = fade(inc0, rate, 100);
      uint8_t g = fade(inc0, rate, 10);
      int s = motorSpeed;
      int d = 1;
    
      // Setting parameters to string
      setR(msg, r);
      setB(msg, b);
      setG(msg, g);
      setRPM(msg, s);
      setDir(msg, d);

      res = msg;
      client.print(res);
      time0 = millis();
      inc0 = inc0 + 1;
    }
  }
}

/**
 * Connection to second client
 */
void *connection1(void *) {

  WiFiClient client;
  char msg[32];
  String res;
  
  while( true ) {

    if(millis() > (delay_time + time1)) {

      if(playing) {

      
        switchPresets(1, inc1);
        int s = motorSpeed;
        int d = 1;
      
        // Setting parameters to string
        setR(msg, r1);
        setB(msg, b1);
        setG(msg, g1);
        setRPM(msg, s);
        setDir(msg, d);
  
        res = msg;
        osc.send(host1, osc_port1, "/client/1", res);
        time1 = millis();
        inc1 = inc1 + 1;

      }
    }

    delay(10);
  }
}

void *touchosc_server (void *) {
  
  osc.subscribe("/1/*", [](OscMessage& m) {
    String addr = m.address();
    String tag = addr.substring(3);
    float val = m.arg<float>(0);
        
    if( tag.equals("preset1") ) {
      if(val == 1.0) {
        preset = 1;
        Serial.println("preset1");
      }
    } else if ( tag.equals("preset2") ) {
      if(val == 1.0) {
        preset = 2;
        Serial.println("preset2");
      }
    } else if ( tag.equals("preset3") ) {
      if(val == 1.0) {
        preset = 3;
        if(randomMode) {
          globalR = random(0, maxSaturation);
          globalG = random(0, maxSaturation);
          globalB = random(0, maxSaturation);
        }
        Serial.println("preset3");
      }
    } else if ( tag.equals("preset4") ) {
      if(val == 1.0) {
        preset = 4;
        Serial.println("preset4");
      }
    } else if ( tag.equals("preset5") ) {
      if(val == 1.0) {
        preset = 5;
        Serial.println("preset5");
      }
    } else if ( tag.equals("speed1") ) {
      // float value from 0.0 to 10.0
      motorSpeed = (int)val;
      Serial.print(motorSpeed);
      Serial.println(" .  motor speed change");
      
    } else if ( tag.equals("speed2") ) {
      ledSpeed = val;
      delay_time = map(val, 0, 10, 1000, 200);
      delay_time = constrain(delay_time, 200, 1000);
      Serial.print(val);
      Serial.println(" led speed change");
      
    } else if ( tag.equals("pause") ) {
      // 1: pause, 0: play
      if(val == 1.0) playing = false;
      else playing = true;
      if(playing) Serial.println("playing");
      else Serial.println("pause");
      
    } else if ( tag.equals("random") ) {
      if(val == 1.0) randomMode = true;
      else randomMode = false;
      if(randomMode) Serial.println("random mode ON");
      else Serial.println("random mode OFF");
      
    } else if ( tag.equals("red") ) {
      // float value from 0.0 to 128.0
      if(!randomMode) {
        globalR = (int)val;
        Serial.print(val);
        Serial.println(" changing red");
      }
      
    } else if ( tag.equals("green") ) {
      if(!randomMode) {
        globalG = (int)val;
        Serial.print(val);
        Serial.println(" changing green");
      }
        
    } else if ( tag.equals("blue") ) {
      if(!randomMode) {
        globalB = (int)val;
        Serial.print(val);
        Serial.println(" changing blue");
      }
        
    } else {
      Serial.println("others");
      
    }      
  }); 

  while (true) {
    osc.parse();
    delay(10);
  }
}

void loop() {}

void switchPresets(int clientNum, int inc) {
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  switch(preset) {
    case 1: {
      
      // synchronized strobe effect with random color
      if(inc % 2 == 0) {
        if(randomMode) {
          r = random(0, maxSaturation);
          g = random(0, maxSaturation);
          b = random(0, maxSaturation);
        } else {
          r = globalR;
          g = globalG;
          b = globalB;
        }
      } else {
        r = 0;
        g = 0;
        b = 0;
      }
      
      break;
    }

    case 2: {

      // alternating strobe effect with random color
      if(clientNum == 0) {
        if(inc % 2 == 0) {
          if(randomMode) {
            r = random(0, maxSaturation);
            g = random(0, maxSaturation);
            b = random(0, maxSaturation);
          } else {
            r = globalR;
            g = globalG;
            b = globalB;
          }
        } else {
          r = 0;
          g = 0;
          b = 0;
        }
      } else {
        if(inc % 2 == 1) {
          if(randomMode) {
            r = random(0, maxSaturation);
            g = random(0, maxSaturation);
            b = random(0, maxSaturation);
          } else {
            r = globalR;
            g = globalG;
            b = globalB;
          }
        } else {
          r = 0;
          g = 0;
          b = 0;
        }
      }
      
      break;
    }

    case 3: {
      
      // fading colors
      r = fade(inc, 0.2, globalR);
      g = fade(inc, 0.2, globalG);
      b = fade(inc, 0.2, globalB);
      Serial.print(r); Serial.print(" "); Serial.print(g); Serial.print(" "); Serial.println(b);
      break;
    }

    case 4: {
      // pulsating or glowing
      
      break;
    }

    case 5: {
      break;
    }
  }

  if(clientNum == 0) {
    r0 = r;
    g0 = g;
    b0 = b;
  } else {
    r1 = r;
    g1 = g;
    b1 = b;
  }
}


/**
 * Fade function
 */
uint8_t fade(int inc, float rate, uint8_t color) {
  uint8_t brightness = 0;
  brightness = (sin((float)inc * rate) + 1) * color / 2;
 
  return brightness;
}

void setR(char msg[32], uint8_t value ) {
  char rd[4];
  itoa(value,rd,10);

  if( value == 0 ) {
    msg[0] = '0';
    msg[1] = '0';
    msg[2] = '0';
  } else if ( value < 10 ) {
    msg[0] = '0';
    msg[1] = '0';
    msg[2] = rd[0];
  } else if (value < 100) {
    msg[0] = '0';
    msg[1] = rd[0];
    msg[2] = rd[1];
  } else {
    msg[0] = rd[0];
    msg[1] = rd[1];
    msg[2] = rd[2];
  }
}

void setB(char msg[32], uint8_t value ) {
  char br[4];
  itoa(value,br,10);

  if( value == 0 ) {
    msg[3] = '0';
    msg[4] = '0';
    msg[5] = '0';
  } else if ( value < 10 ) {
    msg[3] = '0';
    msg[4] = '0';
    msg[5] = br[0];
  } else if (value < 100) {
    msg[3] = '0';
    msg[4] = br[0];
    msg[5] = br[1];
  } else {
    msg[3] = br[0];
    msg[4] = br[1];
    msg[5] = br[2];
  }
}

void setG(char msg[32], uint8_t value ) {

  char gr[4];
  itoa(value,gr,10);

  if( value == 0 ) {
    msg[6] = '0';
    msg[7] = '0';
    msg[8] = '0';
  } else if ( value < 10 ) {
    msg[6] = '0';
    msg[7] = '0';
    msg[8] = gr[0];
  } else if (value < 100) {
    msg[6] = '0';
    msg[7] = gr[0];
    msg[8] = gr[1];
  } else {
    msg[6] = gr[0];
    msg[7] = gr[1];
    msg[8] = gr[2];
  }
  
}

void setRPM(char msg[32], int value ) {
  char sd[4];
  itoa(value,sd,10);

  if( value == 0 ) {
    msg[9] = '0';
    msg[10] = '0';
    msg[11] = '0';
  } else if ( value < 10 ) {
    msg[9] = '0';
    msg[10] = '0';
    msg[11] = sd[0];
  } else if (value < 100) {
    msg[9] = '0';
    msg[10] = sd[0];
    msg[11] = sd[1];
  } else {
    msg[9] = sd[0];
    msg[10] = sd[1];
    msg[11] = sd[2];
  }
}

void setDir(char msg[32], int value ) {
  char d = '0' + value;
  msg[12] = d;
}
