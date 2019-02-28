#include <SPI.h>
#include <WiFi.h>

char ssid[] = "BU Guest (unencrypted)"; //  your network SSID (name)
//char pass[] = "smilingfamily";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

int t = 0;
char msg[32];
char *res;

const uint16_t port = 8090;
const char * host = "10.192.233.102";
//10.192.236.61 -> BU Guest
//10.0.0.44
//const char * host = "10.0.0.44";

WiFiClient client;

void setup() {
  //InitiaIPlize serial and wait for port to open:
  Serial.begin(115200);

 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    //status = WiFi.begin(ssid, pass);
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void loop() {

  // Printing the MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

  // Trying to connect to the host  
  if (!client.connect(host, port)) {

      

      Serial.println("Connection to host failed");

      delay(1000);
      return;
  }

  Serial.println("Connected to server successful!");

  
  /*
   * Testing data to see if it is being sent and recveived
   * Inputting random values for inputs
   * Sending to client side as a string
   * Disconnecting after being sent
   */
  int r = random(0,255);
  int b = random(0,255);
  int g = random(0,255);
  int s = random(0,600);
  int d = t % 2;

  // Setting randome 
  setR(r);
  setB(b);
  setG(g);
  setRPM(s);
  setDir(d);

  Serial.print("Result: ");
  Serial.println(res);

  client.print(res);

  Serial.println("Disconnecting...");
  client.stop();

  delay(10000);
  
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setR( int value ) {
  int digits = 0;
  int temp = value;
  
  while (temp) {
      temp /= 10;
      digits++;
  }

  char rd[4];
  itoa(value,rd,10);

  Serial.print("red: ");
  Serial.println(rd);
  
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
  Serial.println(msg);
  delay(1000);
}

void setB( int value ) {
  int digits = 0;
  int temp = value;
  while (temp) {
      temp /= 10;
      digits++;
  }
  
  char bl[4];
  itoa(value,bl,10);

  Serial.print("blue: ");
  Serial.println(bl);

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
  
  Serial.print("Blue msg: ");
  Serial.println(msg);
  delay(1000);
}

void setG( int value ) {
  int digits = 0;
  int temp = value;
  while (temp) {
      temp /= 10;
      digits++;
  }

  char gr[4];
  itoa(value,gr,10);

  Serial.print("green: ");
  Serial.println(gr);

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
  
  Serial.print("Green msg: ");
  Serial.println(msg);
  delay(1000);
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
  
  Serial.print("Speed msg: ");
  Serial.println(msg);
  delay(1000);
}

void setDir( int value ) {
  char d = '0' + value;
  msg[12] = d;
  Serial.print("Dir msg: ");
  Serial.println(msg);
  res = msg;
  delay(1000);
}
