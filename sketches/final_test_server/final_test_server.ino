#include <pthread.h>
#include <SPI.h>
#include <WiFi.h>

/*
 * Timer function
 */
unsigned long time_1 = 0;
unsigned long time_2 = 0;
unsigned long delay_time = 100;
unsigned long dif;
char ssid[] = "BU Guest (unencrypted)"; //  your network SSID (name)

int t = 0;
char msg[32];
char *res;

const uint16_t port1 = 8090;
const uint16_t port2 = 8095;
const char * host = "10.192.225.110";

const uint16_t maxSaturation = 128;

int r = 0;
int b = 0;
int g = 0;
int rpm = 0;
int dir = 0;

int status = WL_IDLE_STATUS;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid);

    // wait 5 seconds for connection:
    delay(5000);
  }
  Serial.println("Connected to wifi");

  /**
   * Inidialize two threads for each client
   */
  pthread_t threads[2];

  int thread0;
  int thread1;

  int i = 0;
  int j = 1;

  // create thread connection
  thread0 = pthread_create(&threads[0], NULL, connection0, (void *)i);
  //thread1 = pthread_create(&threads[1], NULL, connection1, (void *)j);

  // check to see if error has occured
  if (thread0) {
         Serial.println("An error has occurred thread0");
      }

//  if (thread1) {
//         Serial.println("An error has occurred thread1");
//      }
 
}

/* 
 *  Connection with first client
 */
void *connection0(void *threadId) {

  WiFiClient client;
  
  while( true ) {

      /*
     * Testing data to see if it is being sent and recveived
     * Inputting random values for inputs
     * Sending to client side as a string
     */

    if(millis() > (delay_time + time_2)) {
      
      while (!client.connect(host, 8090)) {
        Serial.println("Connection to host 1 failed");
  
        delay(1000);
      }

      Serial.println("Connected to server 1 successful!");
      
      Serial.println("inside the sending");
      // setting values for 
      float rate = (1.0 / (float)delay_time) * 5.0;   // 5.0 speeds up the fading
      float offset = 0;
      uint16_t r = fade(rate, offset); 
      uint16_t b = fade(rate, 100);
      uint16_t g = fade(rate, 10);
      //uint16_t b = random(0,255);
      //uint16_t g = random(0,255);
      int s = random(0,600);
      int d = random(0,2);
    
      // Setting parameters to string
      setR(r);
      setB(b);
      setG(g);
      setRPM(s);
      setDir(d);

      Serial.print("red: ");
      Serial.println(r);

      Serial.print("blue: ");
      Serial.println(b);

      Serial.print("green: ");
      Serial.println(g);

      Serial.print("speed: ");
      Serial.println(s);
      
      Serial.print("dir: ");
      Serial.println(d);

      res = msg;
      Serial.print("message ");
      Serial.println(res);
      
      client.print(res);
      time_2 = millis();
      t = t + 1;
    }
  }
}

/**
 * Fade function
 */
uint16_t fade(float rate, float offset) {
  uint16_t brightness = 0;
  brightness = (sin(t * rate + offset) + 1) * maxSaturation / 2;

//  Serial.print("Red value in fade function");
//  Serial.println(brightness);
 
  return brightness;
}

/**
 * Second client, still have not tested to see if this works
 */
void *connection1(void *threadId) {

  WiFiClient client;
  
  while (!client.connect(host, port2)) {
      Serial.println("Connection to host 2 failed");

      delay(1000);
  }

  while ( client ) {
    Serial.println("Connected to server 2 successful!");

    client.print("1234567890001");
  }
  
    Serial.println("this is from connection 1");
    Serial.println((int)threadId);
}

void loop() {
}

/*
 * Keep track of how long the time has been elasped since last set
 * 
 */
void setR( uint16_t value ) {
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

void setB( uint16_t value ) {
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

void setG( uint16_t value ) {

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

void setRPM( int value ) {
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

// millis -> returns elapsed time since program starts
// make sure that direction doesnt change too fast
void setDir( int value ) {
  char d = '0' + value;
  msg[12] = d;
}
