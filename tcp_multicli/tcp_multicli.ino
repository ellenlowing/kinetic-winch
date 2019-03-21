#include <pthread.h>
#include <SPI.h>
#include <WiFi.h>

/*
 * Timer function
 */

char ssid[] = "BU Guest (unencrypted)"; //  your network SSID (name)

int t = 0;
char msg[32];
char *res;

const uint16_t port1 = 8090;
const uint16_t port2 = 8095;
const char * host = "10.192.231.179";

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
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    //status = WiFi.begin(ssid, pass);
    status = WiFi.begin(ssid);

    // wait 5 seconds for connection:
    delay(5000);
  }
  Serial.println("Connected to wifi");

  pthread_t threads[2];

  int thread0;
  int thread1;

  int i = 0;
  int j = 1;

  thread0 = pthread_create(&threads[0], NULL, connection0, (void *)i);
  //thread1 = pthread_create(&threads[1], NULL, connection1, (void *)j);

  if (thread0) {
         Serial.println("An error has occurred thread0");
      }

  if (thread1) {
         Serial.println("An error has occurred thread1");
      }
 
}

/* 
 *  
 */

void *connection0(void *threadId) {

  WiFiClient client;
  
  while( true ) {
    while (!client.connect(host, port1)) {
        Serial.println("Connection to host 1 failed");
  
        delay(1000);
    }
    
    Serial.println("Connected to server 1 successful!");

      /*
     * Testing data to see if it is being sent and recveived
     * Inputting random values for inputs
     * Sending to client side as a string
     * Disconnecting after being sent
     */
    // setting values for 
    float rate = 0.0003;
    float zero = 0;
    uint16_t r = fade(rate, zero);
    //uint16_t b = fade(rate, 1);
    //uint16_t g = fade(rate, 2);
    uint16_t b = 0;
    uint16_t g = 0;
    int s = random(0,600);
    int d = random(0,2);
  
    // Setting randome 
    setR(r);
    setB(b);
    setG(g);
    setRPM(s);
    setDir(d);
  
    res = msg;

    Serial.print("Result: ");
    Serial.println(res);

    Serial.println("Sending");
    client.print(res);
  }
}

uint16_t fade(float rate, float offset) {
  uint16_t brightness = 0;
  brightness = (sin(millis() * rate + offset) + 1) * maxSaturation / 2;

  Serial.print("Red value in fade function");
  Serial.println(brightness);
  
  return brightness;
}

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
//  // put your main code here, to run repeatedly:
//  // Printing the MAC address
//  byte mac[6];
//  WiFi.macAddress(mac);
//  Serial.print("MAC address: ");
//  Serial.print(mac[5], HEX);
//  Serial.print(":");
//  Serial.print(mac[4], HEX);
//  Serial.print(":");
//  Serial.print(mac[3], HEX);
//  Serial.print(":");
//  Serial.print(mac[2], HEX);
//  Serial.print(":");
//  Serial.print(mac[1], HEX);
//  Serial.print(":");
//  Serial.println(mac[0], HEX);
  
}

/*
 * Keep track of how long the time has been elasped since last set
 * 
 */
void setR( uint16_t value ) {
  int digits = 0;
  int temp = value;
  
  while (temp) {
      temp /= 10;
      digits++;
  }

  char rd[4];
  itoa(value,rd,10);

//  Serial.print("red: ");
//  Serial.println(rd);
  
  if( digits == 1 ) {
    msg[0] = '0';
    msg[1] = '0';
    msg[2] = rd[0];
  }
  else if ( digits == 2 ) {
    msg[0] = '0';
    msg[1] = rd[0];
    msg[2] = rd[1];
  }
  else {
    msg[0] = rd[0];
    msg[1] = rd[1];
    msg[2] = rd[2];
  }
  
  Serial.print("Red msg: ");
  Serial.print(msg[0]);
  Serial.print(msg[1]);
  Serial.println(msg[2]);
  //delay(1000);
}

void setB( uint16_t value ) {
  int digits = 0;
  int temp = value;
  while (temp) {
      temp /= 10;
      digits++;
  }
  
  char bl[4];
  itoa(value,bl,10);

//  Serial.print("blue: ");
//  Serial.println(bl);

  if( digits == 1 ) {
    msg[3] = '0';
    msg[4] = '0';
    msg[5] = bl[0];
  }
  else if ( digits == 2 ) {
    msg[3] = '0';
    msg[4] = bl[0];
    msg[5] = bl[1];
  }
  else {
    msg[3] = bl[0];
    msg[4] = bl[1];
    msg[5] = bl[2];
  }

  msg[3] = '0';
  msg[4] = '0';
  msg[5] = '0';
  
//  Serial.print("Blue msg: ");
//  Serial.println(msg);
//  delay(1000);
}

void setG( uint16_t value ) {
  int digits = 0;
  int temp = value;
  while (temp) {
      temp /= 10;
      digits++;
  }

  char gr[4];
  itoa(value,gr,10);

//  Serial.print("green: ");
//  Serial.println(gr);

  if( digits == 1 ) {
    msg[6] = '0';
    msg[7] = '0';
    msg[8] = gr[0];
  }
  else if ( digits == 2 ) {
    msg[6] = '0';
    msg[7] = gr[0];
    msg[8] = gr[1];
  }
  else {
    msg[6] = gr[0];
    msg[7] = gr[1];
    msg[8] = gr[2];
  }

  msg[6] = '0';
  msg[7] = '0';
  msg[8] = '0';
  
//  Serial.print("Green msg: ");
//  Serial.println(msg);
//  delay(1000);
}

void setRPM( int value ) {
  int digits = 0;
  int temp = value;
  while (temp) {
      temp /= 10;
      digits++;
  }

  char sd[4];
  itoa(value,sd,10);

  Serial.print("speed: ");
  Serial.println(sd);

  if( digits == 1 ) {
    msg[9] = '0';
    msg[10] = '0';
    msg[11] = sd[0];
  }
  else if ( digits == 2 ) {
    msg[9] = '0';
    msg[10] = sd[0];
    msg[11] = sd[1];
  }
  else {
    msg[9] = sd[0];
    msg[10] = sd[1];
    msg[11] = sd[2];
  }
  
//  Serial.print("Speed msg: ");
//  Serial.println(msg);
//  delay(1000);
}

// millis -> returns elapsed time since program starts
// make sure that direction doesnt change too fast
// 
void setDir( int value ) {
  char d = '0' + value;
  msg[12] = d;
//  Serial.print("Dir msg: ");
//  Serial.println(msg);
//  delay(1000);
}
